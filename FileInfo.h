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

#ifndef FILEINFO_H
#define FILEINFO_H

inline string Normalize(const string &str) {
    size_t sz = str.size();
    string result;
    for (int i = 0; i < sz; i++) {
        char c = str[i];
        switch (c) {
            case ' ':  // whitespace
            case '\t': // whitespace
            case '\r': // platform-specific end-of-line junk
            case '/':  // comment character
            case '*':  // comment character
            case '{':  // tends to vary from style to style
            case '}':  // tends to vary from style to style
                break;

            default:
                result.push_back(c);
        }
    }
    return result;
}

inline bool IsInteresting(const string str) {
    if (str.empty())
        return false;
    else if (str[0] == '#' && str.size() >= 8 && str.substr(0,8) == "#include")
        return false;
    else
        return true;
}

class Shred {
    int m_first_line;
    int m_last_line;
    HashCode m_hash;

public:
    enum { DEFAULT_SIZE = 4 };
    
    Shred(int first_line, int last_line, const string &shred_str)
        : m_first_line(first_line),
          m_last_line(last_line),
          m_hash(shred_str) {}

    const HashCode &GetHashCode() const { return m_hash; }
    int GetFirstLine() const { return m_first_line; }
    int GetLastLine() const  { return m_last_line; }
};

class FileInfo {
    bool m_is_binary;
    vector<Shred> m_shreds;

    void PrepStream(const string &file_name, istream &input);
    bool LooksLikeBinary(const string &file_name);
    void Shredder(const string &file_name, int shred_size);

public:
    typedef vector<Shred>::iterator iterator;
    
    FileInfo(const string &file_name, int shred_size);

    bool IsBinary()  { return m_is_binary; }

    iterator begin() { return m_shreds.begin(); }
    iterator end()   { return m_shreds.end(); }
};


#endif // FILEINFO_H
