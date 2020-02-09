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

#ifndef SOURCETREECHECKER_H
#define SOURCETREECHECKER_H

#ifndef DIRWALKER_H
#include "DirWalker.h"
#endif // ndef DIRWALKER_H

#ifndef SOURCETREEINFO_H
#include "SourceTreeInfo.h"
#endif // ndef SOURCETREEINFO_H

class SourceTreeChecker : public DirWalker {
    const SourceTreeInfo *m_check_against;
    int m_shred_size;

public:
    SourceTreeChecker(const SourceTreeInfo *check_against,
                      int shred_size)
        : m_check_against(check_against), m_shred_size(shred_size) {}

    void DoReportMatch(const LineRange &range,
                       vector<LineRange> &matches);
    void MergeAdjacentRanges(vector<LineRange> &ranges);

protected:
    void ProcessFile(const string &path);

    virtual void ReportHit(const LineRange &range1,
                           const LineRange &range2) = 0;
    virtual void ReportMatch(const LineRange &range,
                             const vector<LineRange> &matches) = 0;
};


#endif // SOURCETREECHECKER_H
