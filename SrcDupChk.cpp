// srcdupchk - Program to find duplication between two source trees.
// Copyright © 2003–2020 Eric Kidd
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
#include "SourceTreeChecker.h"
#include "CsvQuote.h"

class SimpleChecker : public SourceTreeChecker {
    bool m_show_shreds;

public:
    SimpleChecker(const SourceTreeInfo *check_against, int shred_size,
                  bool show_shreds);

protected:
    void ReportHit(const LineRange &range1, const LineRange &range2);
    void ReportMatch(const LineRange &range,
                     const vector<LineRange> &matches);
};

SimpleChecker::SimpleChecker(const SourceTreeInfo *check_against, int shred_size,
                             bool show_shreds)
    : SourceTreeChecker(check_against, shred_size),
      m_show_shreds(show_shreds)
{
    // Print our CSV header.
    cout << "tree2,tree2_first,tree2_last,"
         << "tree1,tree1_first,tree1_last"
         << endl;
}

void SimpleChecker::ReportHit(const LineRange &range1,
                              const LineRange &range2)
{
    if (m_show_shreds)
        cerr << "SHREDS:" << range1 << ":" << range2 << endl;
}

void SimpleChecker::ReportMatch(const LineRange &range,
                                const vector<LineRange> &matches)
{
    vector<LineRange>::const_iterator i = matches.begin();
    for (; i != matches.end(); ++i) {
        cout << CsvQuote(range.path)
             << "," << range.first << "," << range.last
             << "," << CsvQuote(i->path)
             << "," << i->first << "," << i->last
             << endl;
    }
}

static void usage(ostream &out, bool should_die = true) {
    out << "Usage: srcdupchk [--show-shreds] "
        << "[--shred-size N] TREE1 TREE2\n\n"
        << "Compare TREE1 and TREE2 for matches using N-line \"shreds\".\n"
        << "--show-shreds will print low-level matches to stderr."
        << flush;
    if (should_die)
        exit(1);
}

int main(int argc, char **argv) {
    // Parse our arguments.
    int i = 1;
    bool show_shreds = false;
    int shred_size = Shred::DEFAULT_SIZE;
    char *tree1 = NULL, *tree2 = NULL;
    while (i < argc) {
        string arg(argv[i]);
        if (arg == "--version") {
            cout << PACKAGE_STRING << endl << flush;
            exit(0);
        } else if (arg == "--help") {
            usage(cout, false);
            exit(0);
        } else if (arg == "--show-shreds") {
            show_shreds = true;
        } else if (arg == "--shred-size") {
            if (!(++i < argc))
                usage(cerr);
            shred_size = atoi(argv[i]);
        } else if (!tree1) {
            tree1 = argv[i];
        } else if (!tree2) {
            tree2 = argv[i];
        }
        i++;
    }

    if (!tree1 || !tree2 || i != argc)
        usage(cerr);

    try {
        SourceTreeInfo info(tree1, shred_size);
        SimpleChecker checker(&info, shred_size, show_shreds);
        checker.Walk(tree2);
    } catch (std::exception &e) {
        cerr << PACKAGE_NAME << ": " << e.what() << endl;
        exit(1);
    } catch (...) {
        cerr << PACKAGE_NAME << ": unknown exception" << endl;
        exit(1);
    }

    return 0;
}
