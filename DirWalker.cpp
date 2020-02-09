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

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void DirWalker::ThrowPosixException(const string &path) {
    throw runtime_error(path + ": " + strerror(errno));
}

void DirWalker::WalkDir(const string &path) {
    DIR *dir = opendir(path.c_str());
    if (!dir)
        ThrowPosixException(path);

    try {
        vector<string> entries;

        // Do the actual directory walk.
        errno = 0;
        struct dirent *entry;
        for (entry = readdir(dir); entry != NULL; entry = readdir(dir)) {
            string entry_name = entry->d_name;
            if (entry_name == "." || entry_name == ".." || entry_name == "CVS")
                continue; // Skip magic UNIX directory entries & CVS junk.
            entries.push_back(entry_name);
        }
        if (errno)
            ThrowPosixException(path);

        // Sort the results.
        sort(entries.begin(), entries.end());

        // Process the individual entries in alphabetical order.
        vector<string>::iterator i;
        for (i = entries.begin(); i != entries.end(); ++i)
            Walk(path + "/" + *i);

    } catch (...) {
        closedir(dir);
        throw;
    }
    if (closedir(dir) < 0)
        ThrowPosixException(path);
}

void DirWalker::Walk(const string &path) {
    struct stat info;
    if (lstat(path.c_str(), &info) < 0)
        ThrowPosixException(path);

    // Process directories and regular files.  Ignore sockets, devices,
    // symlinks, FIFOs, and all the other crufty things which can appear in
    // a UNIX file system.
    if (info.st_mode & S_IFDIR)
        WalkDir(path);
    else if (info.st_mode & S_IFREG)
        ProcessFile(path);
}
