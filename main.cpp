#include "MidiFile.h"
#include "Options.h"
#include <iostream>
#include <iomanip>

using namespace std;
using namespace smf;

int main(int argc, char** argv) {
  if (argc != 3) {
    cout << "usage: " << argv[0] << " IN OUT" << endl;
    return 1;
  }

  MidiFile midifile;
  midifile.read(argv[1]);

  midifile.doTimeAnalysis();
  midifile.sortTracks();
  midifile.joinTracks();
  const auto &track = midifile[0];
  // use a new track since old track is used for sec -> tick calculation
  auto new_track = midifile[0]; 

  int tick_shift = 0;
  double max_event_distance = 10;
  double last_event_orig_sec = 0;
  for (int i = 0; i < track.size(); ++i) {
    if (track[i].seconds > last_event_orig_sec + max_event_distance) {
      tick_shift +=
        track[i].tick
        - midifile.getAbsoluteTickTime(last_event_orig_sec + max_event_distance);
    }
    last_event_orig_sec = track[i].seconds;
    new_track[i].tick = track[i].tick - tick_shift;
  }

  midifile[0] = new_track;

  midifile.splitTracks();
  midifile.write(argv[2]);

  return 0;
}
