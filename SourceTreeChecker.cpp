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

void SourceTreeChecker::ProcessFile(const string &path) {
    FileInfo file_info(path, m_shred_size);

    int match_start = 0, match_end_candidate;
    FileInfo::iterator i = file_info.begin();
    LineRange range;
    vector<LineRange> matches;
    for (; i != file_info.end(); i++) {
        if (m_check_against->Find(i->GetHashCode(), range)) {
            matches.push_back(range);
            ReportHit(range, LineRange(path, i->GetFirstLine(),
                                       i->GetLastLine()));
            if (!match_start)
                match_start = i->GetFirstLine();
            match_end_candidate = i->GetLastLine();
        } else if (match_start) {
            DoReportMatch(LineRange(path, match_start, match_end_candidate),
                        matches);
            match_start = 0;
        }
    }
    if (match_start)
        DoReportMatch(LineRange(path, match_start, match_end_candidate),
                      matches);
}

void SourceTreeChecker::DoReportMatch(const LineRange &range,
                                      vector<LineRange> &matches)
{
    MergeAdjacentRanges(matches);
    ReportMatch(range, matches);
    matches.clear();
}

void SourceTreeChecker::MergeAdjacentRanges(vector<LineRange> &ranges) {
    assert(ranges.begin() != ranges.end());
    sort(ranges.begin(), ranges.end());

    vector<LineRange> merged;
    vector<LineRange>::iterator i = ranges.begin();
    LineRange current = *i++;
    for (; i != ranges.end(); ++i) {
        if (i->path == current.path && i->first <= current.last+1) {
            if (i->last > current.last) {
                current.last = i->last;
            }
        } else {
            merged.push_back(current);
            current = *i;
        }
    }
    merged.push_back(current);

    ranges = merged;
}
