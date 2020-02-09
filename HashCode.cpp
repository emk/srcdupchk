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
#include "HashCode.h"

extern "C" {
#include "md5.h"
};

HashCode::HashCode(const string &str_to_hash) {
    md5_buffer(str_to_hash.c_str(), str_to_hash.size(), (void *) &m_hash[0]);
}

string HashCode::AsString() const {
    size_t sz = BYTES_IN_MD5_SUM * 2 + 1;
    char buffer[sz];
    for (int i = 0; i < BYTES_IN_MD5_SUM; i++)
        snprintf(&buffer[2 * i], 3, "%02x", m_hash[i]);
    return buffer;
}
