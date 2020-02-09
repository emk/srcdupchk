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
#include "DirWalker.h"
#include "SourceTreeInfo.h"

bool LineRange::operator==(const LineRange &right) const {
    return (path == right.path &&
            first == right.first &&
            last == right.last);
}

bool LineRange::operator<(const LineRange &right) const {
    return (path < right.path ||
            path == right.path && (first < right.first ||
                                   first == right.first && last < right.last));
}

ostream &operator<<(ostream &out, const LineRange &range) {
    out << range.path << ":" << range.first << ":" << range.last;
    return out;
}

class SourceTreeInfoBuilder : public DirWalker {
    SourceTreeInfo *m_info;
    int m_shred_size;

public:
    SourceTreeInfoBuilder(SourceTreeInfo *info, int shred_size)
        : m_info(info), m_shred_size(shred_size) {}
    void ProcessFile(const string &path);
};

void SourceTreeInfoBuilder::ProcessFile(const string &path) {
    FileInfo file_info(path, m_shred_size);
    FileInfo::iterator i = file_info.begin();
    for (; i != file_info.end(); i++)
        m_info->AddShred(path, *i);
};

SourceTreeInfo::SourceTreeInfo(const string &path, int shred_size) {
    SourceTreeInfoBuilder builder(this, shred_size);
    builder.Walk(path);
}

void SourceTreeInfo::AddShred(const string &path, const Shred &shred) {
    m_map.insert(ShredMap::value_type(shred.GetHashCode(),
                                      LineRange(path, shred.GetFirstLine(),
                                                shred.GetLastLine())));
}

bool SourceTreeInfo::Find(const HashCode &hash, LineRange &out_range) const
{
    ShredMap::const_iterator found = m_map.find(hash);
    if (found == m_map.end()) {
        return false;
    } else {
        out_range = found->second;
        return true;
    }
}
