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

class SimpleChecker : public SourceTreeChecker {
    bool m_show_shreds;
    bool m_show_matches;

public:
    SimpleChecker(const SourceTreeInfo *check_against, int shred_size,
                  bool show_shreds, bool show_matches)
        : SourceTreeChecker(check_against, shred_size),
          m_show_shreds(show_shreds), m_show_matches(show_matches) {}

protected:
    void ReportHit(const LineRange &range1, const LineRange &range2);
    void ReportMatch(const LineRange &range,
                     const vector<LineRange> &matches);
};

void SimpleChecker::ReportHit(const LineRange &range1,
                              const LineRange &range2)
{
    if (m_show_shreds)
        cout << "SHREDS:" << range1 << ":" << range2 << endl;
}

void SimpleChecker::ReportMatch(const LineRange &range,
                                const vector<LineRange> &matches)
{
    cout << range << endl;
    if (m_show_matches) {
        vector<LineRange>::const_iterator i = matches.begin();
        for (; i != matches.end(); ++i) {
            cout << "MATCH:" << range << ":" << *i << endl;
        }
    }
}

static void usage(ostream &out, bool should_die = true) {
    out << "Usage: srcdupchk [--show-shreds] [--show-matches] "
        << "[--shred-size N] TREE1 TREE2\n\n"
        << "Compare TREE1 and TREE2 for matches using N-line \"shreds\".\n"
        << "Unless '--show-shreds' or '--show-matches' is specified, the\n"
        << "output only includes file names and line numbers for TREE2.\n"
        << flush;
    if (should_die)
        exit(1);
}

int main(int argc, char **argv) {
    // Parse our arguments.
    int i = 1;
    bool show_shreds = false, show_matches = false;
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
        } else if (arg == "--show-matches") {
            show_matches = true;
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
        SimpleChecker checker(&info, shred_size, show_shreds, show_matches);
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
