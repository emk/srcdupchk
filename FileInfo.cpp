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
#include "HashCode.h"
#include "FileInfo.h"

static const int HEADER_SIZE = 512;
static const double MAX_HIGHBIT_RATIO_IN_TEXT = 0.1;

FileInfo::FileInfo(const string &file_name, int shred_size) {
    m_is_binary = LooksLikeBinary(file_name);
    if (!m_is_binary)
        Shredder(file_name, shred_size);
}

void FileInfo::PrepStream(const string &file_name, istream &input) {
    if (!input)
        throw runtime_error("Cannot open " + file_name);

    // Turn on useful exceptions.  We used to throw exceptions for
    // ios::failbit, but recent versions of the GNU C++ runtime library set
    // ios::failbit when istream::read can't fill its buffer and when
    // getline encounters an end-of-file.
    input.exceptions(/*ios::failbit |*/ ios::badbit);
}

bool FileInfo::LooksLikeBinary(const string &file_name) {
    ifstream input(file_name.c_str(), ios::binary);
    PrepStream(file_name, input);

    char buffer[HEADER_SIZE];
    input.read(buffer, HEADER_SIZE);
    size_t sz = input.gcount();

    // Any '\0' characters make us automatically binary, as does too high
    // a ratio of characters with their high bit set.
    int high_bit_count = 0;
    for (size_t i = 0; i < sz; i++) {
        if (buffer[i] == '\0')
            return true;
        else if (buffer[i] & 0x80)
            high_bit_count++;
    }
    return (1.0 * high_bit_count / sz > MAX_HIGHBIT_RATIO_IN_TEXT);
}

void FileInfo::Shredder(const string &file_name, int shred_size) {
    ifstream input(file_name.c_str());
    PrepStream(file_name, input);

    // This algorithm was proposed by an anonymous reader of the Inquirer.

    string line;
    vector<string> line_buffer(shred_size);
    vector<int> line_no_buffer(shred_size);
    int line_no = 1;    // Starts numbering lines at 1, like GCC does.
    int line_index = 0; // Starts at 0, skips uninteresting lines.

    for (; !input.eof(); line_no++) {
        getline(input, line);
        line = Normalize(line);
        if (!IsInteresting(line))
            continue;

        int circular_index = line_index % shred_size;
        line_buffer[circular_index] = line;
        line_no_buffer[circular_index] = line_no;

        if (line_index + 1 >= shred_size) {
            string shred;
            for (int i = 1; i <= shred_size; i++) {
                shred += line_buffer[(line_index+i) % shred_size];
                shred += "\n";
            }
            int first = line_no_buffer[(line_index+1) % shred_size];
            int last  = line_no_buffer[circular_index];
            m_shreds.push_back(Shred(first, last, shred));
        }
        line_index++;
    }
}
