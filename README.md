# What Is This Branch?

The [open5gs project](https://github.com/open5gs/open5gs) is currently in a holding pattern with respect to exporting system metrics (e.g. number of currently attached eNB/UE) for parsing/integration with other networking systems. Until the open5gs team is able to decide exactly how they want to export these metrics, and have the resources to maintain them, I (@spencersevilla) wrote my own simple version and will be maintaining it on this fork. Aside from my (very minor) changes, this fork is equivalent to the `main` branch of `open5gs`. I have only tested that it works for `apt` package installations for Ubuntu 20.04, but I believe it should be somewhat generalizeable.

## Metrics Exported

We export metrics the same basic way the Linux Kernel exposes information in `/proc`: by writing variables as named files under the directory `/tmp/open5gs/`, and overwriting/updating the files as these values change over time. Reading the content of these files should always give you the most up-to-date value of the variable in question.

The files `{service}_start_time` (e.g. `/tmp/open5gs/mme_start_time`) are comprised of two lines. The first line is the date-time parsed for human readibility (e.g. `Mon Jul 12 16:32:00 2021`) and the second line is the raw seconds since the Unix epoch (e.g. `1626107520`).

For now, the other files are `connected_enbs`, `connected_ues`, and `attached_ues`. They each contain a single number that corresponds to the static variable used for stats and seen in the log. I am planning on adding some additional information regarding sessions as well as UE-specific information (e.g. the IP address belonging to a UE).

The reason I chose to use files is beacuse it's Unix-esque, it's completely agnostic to any specific network monitoring platform, and it should be easy to write a shim for whatever platform you want to integrate against. If you or your org have specific metrics you would like to see exposed, please reach out to me (or just issue a PR) and I will add them.

## Building and Installing

Building is identical to how you normally build open5gs, see https://open5gs.org/open5gs/docs/guide/02-building-open5gs-from-sources/. If you want to just download and install a package for Ubuntu 20.04, I will host it and provide a link to download soon.
