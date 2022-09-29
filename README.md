# What Is This Repo?

This is our (Althea's) branch of [open5gs](https://github.com/open5gs/open5gs). Our team is firmly committed to open-sourcing and upstreaming all our contributions to open5gs whenever possible and desired by the community; this repo only holds commits that are either (a) still under active testing/development or (b) not desired by the greater open5gs community. Every change from upstream open5gs will be explained/listed below.

## Number of Served TAC/TAI
open5gs has a limit of 16 different served TAIs hard-coded in `OGS_MAX_NUM_OF_SERVED_TAI`. Althea's KeyLTE architecture relies on many more than that: each edge KeyLTE router has its own TAC/TAI. We have currently set `OGS_MAX_NUM_OF_SERVED_TAI` to 256; this might increase again in the future.

## PFCP Send_ASR Option
When configuring a node's PFCP connection to another node, you now have the option of adding `send_asr: false` to the configuration yaml. This bool (which defaults to `true`) indicates whether the node should send PFCP AssociationSetupRequests or not.

The reason we need this bool/option is to turn it off for the CPS side. To allow KeyLTE routers to be seamlessly added to the existing CPS without requiring a reboot, we have to define all 256 served TAIs in the CPS configuration. Without this option, the CPS logs are full of errors trying to send ASR messages to KeyLTE routers that don't yet exist.

## Metrics Exported
We export metrics the same basic way the Linux Kernel exposes information in `/proc`: by writing variables as named files under the directory `/tmp/open5gs/`, and overwriting/updating the files as these values change over time. Reading the content of these files should always give you the most up-to-date value of the variable in question.

The files `{service}_start_time` (e.g. `/tmp/open5gs/mme_start_time`) are comprised of two lines. The first line is the date-time parsed for human readibility (e.g. `Mon Jul 12 16:32:00 2021`) and the second line is the raw seconds since the Unix epoch (e.g. `1626107520`).

Each service also has its own subdirectory (e.g. `/tmp/open5gs/mme/`) that contains exports other information/values. The files with a `num` in the front (e.g. `/tmp/open5gs/mme/num_enbs`) contain a single number that indicates how many of that item the service is connected to. The files with a `list` in front (e.g. `/tmp/open5gs/mme/list_enbs`) contains more information about each of those connected items. Each item has its own line (i.e. the number of lines in `list_enbs` should equal `num_enbs`), and each line has a series of key-value information separated by spaces (e.g. `ip:10.0.0.2 tac:2`).

The reason I chose to use files is beacuse it's Unix-esque, it's completely agnostic to any specific network monitoring platform, and it should be easy to write a shim for whatever platform you want to integrate against. If you or your org have specific metrics you would like to see exposed, please reach out to me (or just issue a PR) and I will add them.
