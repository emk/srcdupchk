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

#ifndef HASHCODE_H
#define HASHCODE_H

class HashCode {
    enum { BYTES_IN_MD5_SUM = 16 };
    unsigned char m_hash[BYTES_IN_MD5_SUM];

public:
    explicit HashCode(const string &str_to_hash);
    string AsString() const;

    bool operator== (const HashCode &right) const;
    bool operator<  (const HashCode &right) const;
};

inline bool HashCode::operator==(const HashCode &right) const {
    return memcmp(m_hash, right.m_hash, BYTES_IN_MD5_SUM) == 0;
}

inline bool HashCode::operator<(const HashCode &right) const {
    return memcmp(m_hash, right.m_hash, BYTES_IN_MD5_SUM) < 0;
}

#endif // HASHCODE_H
