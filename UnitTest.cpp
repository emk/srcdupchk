// -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4; -*-
//
// srcdupchk - Program to find duplication between two source trees.
// Copyright (C) 2003 Eric Kidd <eric.kidd@pobox.com>
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include "DcHeaders.h"
#include "UnitTest.h"

#include "HashCode.h"
#include "FileInfo.h"
#include "DirWalker.h"
#include "SourceTreeInfo.h"
#include "SourceTreeChecker.h"

static void test_HashCode() {
    CHECK_EQ(HashCode("Hello").AsString(), "8b1a9953c4611296a827abf8c47804d7");
    CHECK_EQ(HashCode("World").AsString(), "f5a7924e621e84c9280a9a27e1bcb7f6");
    CHECK_EQ(HashCode("X") == HashCode("X"), true);
    CHECK_EQ(HashCode("X") == HashCode("Y"), false);

    // I just so happen to know the ordering of these two hash codes.
    CHECK_EQ(HashCode("X") < HashCode("Y"), true);
    CHECK_EQ(HashCode("Y") < HashCode("X"), false);
}

static void test_FileInfo() {

    // Source line analysis.
    CHECK_EQ(Normalize(""), "");
    CHECK_EQ(Normalize(" a\tb/c*d{e}f\rg"), "abcdefg");
    CHECK_EQ(IsInteresting(""), false);
    CHECK_EQ(IsInteresting("#includexyz"), false);
    CHECK_EQ(IsInteresting("#inc"), true);
    CHECK_EQ(IsInteresting("anything else"), true);

    // Shredding and checksumming.
    FileInfo info("test1/a.txt", 4);
    CHECK_EQ(info.IsBinary(), false);
    CHECK_EQ(info.begin() + 3, info.end());
    FileInfo::iterator i = info.begin();
    CHECK_EQ(i->GetFirstLine(), 1);
    CHECK_EQ(i->GetLastLine(),  6);
    CHECK_EQ(i++->GetHashCode(), HashCode("FOO\nBAR\nBAZ\nMOBY\n"));
    CHECK_EQ(i->GetFirstLine(), 2);
    CHECK_EQ(i->GetLastLine(),  7);
    CHECK_EQ(i++->GetHashCode(), HashCode("BAR\nBAZ\nMOBY\nQUUX\n"));
    CHECK_EQ(i->GetFirstLine(), 3);
    CHECK_EQ(i->GetLastLine(),  9);
    CHECK_EQ(i++->GetHashCode(), HashCode("BAZ\nMOBY\nQUUX\nQUUUX\n"));
    CHECK_EQ(i, info.end());

    // Binary file identification.
    FileInfo info2("test1/arrow.png", 4);
    CHECK_EQ(info2.IsBinary(), true);
    CHECK_EQ(info2.begin(), info2.end());
}

class DummyDirWalker : public DirWalker {
    int m_file_index;

public:
    enum { TEST1_FILE_COUNT = 4 };

    DummyDirWalker() : m_file_index(0) {}
    bool AllFilesProcessed() { return m_file_index == TEST1_FILE_COUNT; }
    void ProcessFile(const string &path);
};

void DummyDirWalker::ProcessFile(const string &path) {
    char *test1_files[TEST1_FILE_COUNT] = {
        "test1/a.txt",
        "test1/arrow.png",
        "test1/b.txt",
        "test1/subdir/c.txt"
    };

    CHECK_EQ(0 <= m_file_index && m_file_index < TEST1_FILE_COUNT, true);
    CHECK_EQ(path, test1_files[m_file_index]);

    m_file_index++;
}

static void test_DirWalker() {
    DummyDirWalker walker;
    walker.Walk("test1");
    CHECK_EQ(walker.AllFilesProcessed(), true);
}

static void test_SourceTreeInfo() {
    LineRange range;

    SourceTreeInfo info("test1", 4);

    HashCode missing("no\nsuch\nlines\nexist\n");
    CHECK_EQ(info.Find(missing, range), false);

    HashCode present("quick\nbrown\nfox\njumped\n");
    CHECK_EQ(info.Find(present, range), true);
    CHECK_EQ(range, LineRange("test1/subdir/c.txt", 2, 5));
}

class DummySourceTreeChecker : public SourceTreeChecker {
    int m_hit;
    int m_match;

public:
    DummySourceTreeChecker(const SourceTreeInfo *check_against, int shred_size)
        : SourceTreeChecker(check_against, shred_size), m_hit(0), m_match(0) {}
    bool Finished() { return m_hit == 3 && m_match == 2; }

protected:
    void ReportHit(const LineRange &range1, const LineRange &range2);
    void ReportMatch(const LineRange &range,
                     const vector<LineRange> &matches);
};

void DummySourceTreeChecker::ReportHit(const LineRange &range1,
                                       const LineRange &range2)
{
    switch (m_hit++) {
        case 0:
            CHECK_EQ(range1, LineRange("test1/subdir/c.txt", 1, 4));
            CHECK_EQ(range2, LineRange("test2/odd.txt", 3, 6));
            break;
        case 1:
            CHECK_EQ(range1, LineRange("test1/subdir/c.txt", 2, 5));
            CHECK_EQ(range2, LineRange("test2/odd.txt", 4, 7));
            break;
        case 2:
            CHECK_EQ(range1, LineRange("test1/subdir/c.txt", 6, 9));
            CHECK_EQ(range2, LineRange("test2/odd.txt", 11, 14));
            break;
        default:
            FAIL_TEST("m_hit out of range");
    }
}

void DummySourceTreeChecker::ReportMatch(const LineRange &range,
                                         const vector<LineRange> &matches)
{
    CHECK_EQ(matches.size(), 1);
    switch (m_match++) {
        case 0:
            CHECK_EQ(range, LineRange("test2/odd.txt", 3, 7));
            CHECK_EQ(matches[0], LineRange("test1/subdir/c.txt", 1, 5));
            break;
        case 1:
            CHECK_EQ(range, LineRange("test2/odd.txt", 11, 14));
            CHECK_EQ(matches[0], LineRange("test1/subdir/c.txt", 6, 9));
            break;
        default:
            FAIL_TEST("m_match out of range");
    }
}

static void test_SourceTreeChecker() {
    SourceTreeInfo info("test1", 4);

    DummySourceTreeChecker checker(&info, 4);
    checker.Walk("test2");
    CHECK_EQ(checker.Finished(), true);
}

int main(int argc, char **argv) {
    try {

        test_HashCode();
        test_FileInfo();
        test_DirWalker();
        test_SourceTreeInfo();
        test_SourceTreeChecker();

    } catch (std::exception &e) {
        cerr << PACKAGE_NAME << ": " << e.what() << endl;
        exit(1);
    } catch (...) {
        cerr << PACKAGE_NAME << ": unknown exception" << endl;
        exit(1);
    }

    cout << endl;
    return 0;
}
