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

#ifndef SOURCETREEINFO_H
#define SOURCETREEINFO_H

#ifndef HASHCODE_H
#include "HashCode.h"
#endif // ndef HASHCODE_H

#ifndef FILEINFO_H
#include "FileInfo.h"
#endif // ndef FILEINFO_H

struct LineRange {
    string path;
    int first;
    int last;

    LineRange() : first(0), last(0) {}
    LineRange(const string &_path, int _first, int _last)
        : path(_path), first(_first), last(_last) {}

    bool operator==(const LineRange &right) const;
    bool operator<(const LineRange &right) const;
};

extern ostream &operator<<(ostream &out, const LineRange &range);

class SourceTreeInfo {
    friend class SourceTreeInfoBuilder;

    typedef map<HashCode,LineRange> ShredMap;

    ShredMap m_map;

    void ProcessFile(const string &path);

    void AddShred(const string &path, const Shred &shred);

public:
    SourceTreeInfo(const string &path, int shred_size);
    bool Find(const HashCode &hash, LineRange &out_range) const;
};

#endif // SOURCETREEINFO_H
