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

#ifndef UNITTEST_H
#define UNITTEST_H

#define FAIL_TEST(MSG) \
    do { \
        cout << "\n" << __FILE__ << ":" << __LINE__ << ": " << MSG << "\n"; \
        abort(); \
    } while (0)

#define CHECK_EQ(EXPR1,EXPR2) \
    do { \
        if (EXPR1 == EXPR2) \
            cout << "."; \
        else { \
            cout << "\n" << __FILE__ << ":" << __LINE__ \
                 << ": Expected " #EXPR1 " == " #EXPR2 "\n"; \
	    abort(); \
        } \
    } while (0)

#endif // UNITTEST_H

