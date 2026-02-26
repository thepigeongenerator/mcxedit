> [!CAUTION]
> This application is __not__ ready for usage, and shouldn't be used.
> It is under active development and most features are considered "béta",
> Once a pre-release, or release has been created in [the release tab](https://github.com/thepigeongenerator/mcxedit/releases),
> it is ready for general usage and testing.

[![licence](https://img.shields.io/github/license/thepigeongenerator/mcxedit.svg)](https://github.com/thepigeongenerator/mcxedit/blob/main/COPYING)
[![issues](https://img.shields.io/github/issues/thepigeongenerator/mcxedit.svg)](https://github.com/thepigeongenerator/mcxedit/issues/)

## What does it do
MCX Edit is a tool used to edit [region files](https://minecraft.wiki/w/Region_file_format)
of your [Minecraft Java](https://minecraft.wiki/w/Java_Edition) worlds.
This is useful for reducing world size, general world management,
perhaps copying chunks from one world to another or various other administrative tasks.
Although not the intended main use case, it is also possible to use this tool to view the world map.
Though, it is recommended to use something like [dynmap](https://dynmap.wiki.gg/wiki/Home) instead (if applicable).

# Roadmap
- [ ] First, I shall focus on writing code to view
  chunks, creating a map of the blocks. The code shall
  produce a [ppm](https://en.wikipedia.org/wiki/Netpbm) file of a respective Y level.
  This is to verify whether the binary parsing works correctly. It is intended to support
  both `.mca` and `.mcr` files, and various levels of compression.
- [ ] This shall expand into a "mode" that filters the chunks based on player time,
  further testing the NBT parsing capabilities, and deletion of chunks.
- [X] Also pruning unused sectors, such as in-between entries and towards the end.
- [ ] A GUI shall be built, which is still unclear how this'll be implemented.
  For starters, the GUI shall purely provide viewing capabilities, at which point the roadmap
  shall be updated for features such as chunk selection, et cetera.

# Miscellaneous planned features:
*note that these entries aren't guaranteed to be implemented.
Due to their viability and usefulness still needing to be measured.*
- Changing chunk compression
- View chunk metadata
- Change chunk sNBT
- "Upgrading" `.mcr` regions to `.mca`.

# Similar software
* [MCA-Selector](https://github.com/Querz/mcaselector) (inspiration of this project)
