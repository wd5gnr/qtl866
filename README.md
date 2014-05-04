qtl866 - GUI driver for minipro EPROM/Device programmer software
    See also https://github.com/vdudouyt/minipro
    Copyright (C) 2014  Al Williams al.williams@awce.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

There are two parts to this:
1) A shell script called miniprohex

The script is smart enough to determine the file type from the suffix
(.hex or .srec) and uses srec_cat to convert back and forth.

miniprohex by Al Williams http://www.awce.com
Usage:
  miniprohex [--o offset] [--unfill byte size] [--obs blksize] [--line-length length] [minipro_options] -r filename.ext
  miniprohex [--o offset] [minirpo_options] -w filename.ext

This calls minipro after converting known file types to 
.bin for writing or converting bin files after reading.

--o: Offset for file conversion (see srec_cat)
--unfill: Unfil blocks of at least size of byte (see srec_cat)
--obs: Output block size (see srec_cat)
--line-length: Output line length max (see srec_cat)

Assumes minipro and srec_cat are on the path.

2) A QT GUI for minipro
You need QTCreator to compile -- just open the .pro file and run it 
or compile from inside QTCreator.

By default, the GUI calls minipro (which it expects on the path).
But the options dialog lets you set a path, or you can point
to minipro.hex to get the features of miniprohex.

You can also add options there such as: miniprohex --obs 16


