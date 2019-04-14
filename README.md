# Sharkive

Gateshark cheat code database for Luma3DS 9.1+ and Atmosphere 0.8.5+.

## Database

Sharkive relies on a manually populated database to download and apply new cheats. Most of them are not yet confirmed to work.

**We need your help**: report all the cheats that aren't working as an issue in this repository. We will update them or remove them to avoid further issues with other users. Additionally, if you have cheats that aren't available in Sharkive yet, please create a Pull Request.

## FAQ
**Q:** Help! This cheat doesn't work.<br>
**Q:** Why does the name turn red when I try to use this cheat?<br>
**A:** There can be a few causes for cheats not working:<br>
1. There are bugs in the cheat engine of the latest release of Luma (as of this writing). A number of these are fixed in more recent nightly builds of Luma, which can be found [here](https://github.com/hax0kartik/luma-hourlies/releases/latest)
2. Cheats are all made for a particular version (game update) and/or revision of a game and will likely not work on others. Make sure the cheat(s) you are trying to use are for the version and/or revision you are playing.
3. Most of the cheats in Sharkive's database came from [JourneyOver's collection](https://github.com/JourneyOver/CTRPF-AR-CHEAT-CODES) which includes some Action Replay codes that aren't compatible with Luma's cheat engine.
4. If after trying the above you still can't get the cheat(s) to work, please [report them](https://github.com/FlagBrew/Sharkive/issues/new) so we can either relabel them for their proper version/revision or remove them.

**Q:** Can you add cheats for this game?<br>
**A:** Nobody currently on the FlagBrew team makes cheats. Everything in Sharkive's database comes from user submissions. If you have some working cheats for your game and want them included in Sharkive, submit a [pull request](https://github.com/FlagBrew/Sharkive/pulls) and someone will get around to adding them.

**Q:** Does this work for DS games?<br>
**A:** No. Luma's Rosalina cannot interact with DS games so its cheat engine is unavailable to them. You can try looking into [NitroHax](https://github.com/chishm/nitrohax) for DS cheats.

**Q:** Some cheats have a star (`*`) next to their name in Rosalina. Why?<br>
**A:** This means that the cheat makes use of a key combo. If the cheat name doesn't mention the key combo you'll have to look up the code and translate the line(s) starting `DD000000` to figure out what the key combo is.

**Q:** Some cheat names are cut off in the list. Can you make the full name display? Can I read the full name somewhere?<br>
**A:** Cheat names getting cut off is due to Luma, not Sharkive. The only thing we can potentially do is shorten the cheat's name. You can read the full cheat name by looking up the cheat file located either on your SD or in [Sharkive's database](https://github.com/FlagBrew/Sharkive/tree/master) (you'll need your game's title ID in both cases).

## License

This project is licensed under the GNU GPLv3. See [LICENSE.md](https://github.com/FlagBrew/Sharkive/blob/master/LICENSE) for details.

## Credits

JourneyOver for providing and maintaining all the cheat codes in Sharkive's database.

All the cheats submitters and reporters.

All Luma3DS contributors.