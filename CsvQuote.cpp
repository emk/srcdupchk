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
#include "CsvQuote.h"

ostream &operator<<(ostream &out, const CsvQuote &quote) {
    if (quote.m_value.find(',') != string::npos
        || quote.m_value.find('"') != string::npos)
    {
        out << '"';
        std::size_t range_begin = 0;
        std::size_t range_end = quote.m_value.find('"');
        while (range_end != string::npos) {
            // We found a quote at `range_end`, so output our original text
            // plus two quotes.
            out << quote.m_value.substr(range_begin, range_end - range_begin);
            out << "\"\"";

            // Search again.
            range_begin = range_end + 1;
            range_end = quote.m_value.find('"', range_begin);
        }
        out << quote.m_value.substr(range_begin, quote.m_value.length() - range_begin);
        out << '"';
    } else {
        out << quote.m_value;
    }
}
