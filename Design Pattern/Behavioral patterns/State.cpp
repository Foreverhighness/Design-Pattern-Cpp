#include <cassert>
#include <iostream>
#include <memory>
using namespace std;

class AudioPlayer;
class State {
 public:
  // 上下文将自身传递给状态构造函数。这可帮助状态在需要时获取一些有用的上下文数据。
  explicit State(AudioPlayer& player) : player{player} {}
  virtual ~State() = default;
  virtual void clickLock() = 0;
  virtual void clickPlay() = 0;
  virtual void clickNext() = 0;
  virtual void clickPrevious() = 0;
  friend class AudioPlayer;

 protected:
  AudioPlayer& player;
};

class ReadyState : public State {
 public:
  explicit ReadyState(AudioPlayer& player) : State{player} { cout << "switch to ready state\n"; }
  virtual ~ReadyState() = default;
  void clickLock() override;
  void clickPlay() override;
  void clickNext() override;
  void clickPrevious() override;
};

class PlayingState : public State {
 public:
  explicit PlayingState(AudioPlayer& player) : State{player} { cout << "switch to playing state\n"; }
  virtual ~PlayingState() = default;
  void clickLock() override;
  void clickPlay() override;
  void clickNext() override;
  void clickPrevious() override;
};

class LockedState : public State {
 public:
  explicit LockedState(AudioPlayer& player) : State{player} { cout << "switch to locked state\n"; }
  virtual ~LockedState() = default;
  void clickLock() override;
  void clickPlay() override;
  void clickNext() override;
  void clickPrevious() override;
};
// 音频播放器（Audio­Player）类即为上下文。它还会维护指向状态类实例的引用，
// 该状态类则用于表示音频播放器当前的状态。
class AudioPlayer {
 public:
  unique_ptr<State> state;
  int volume{100}, currentSong{0};
  bool playing{false};
  AudioPlayer() { state = make_unique<ReadyState>(*this); }
  void changeState(unique_ptr<State>&& state) {
    assert(&state->player == this);
    this->state = std::move(state);
  }

  void clickLock() { state->clickLock(); }
  void clickPlay() { state->clickPlay(); }
  void clickNext() { state->clickNext(); }
  void clickPrevious() { state->clickPrevious(); }

  void startPlayback() {
    cout << "start playing\n";
    playing = true;
  }
  void stopPlayback() {
    cout << "stop playing\n";
    playing = false;
  }
  void nextSong() {
    cout << "next song\n";
    currentSong += 1;
  }
  void previousSong() {
    cout << "previous song\n";
    currentSong -= 1;
  }
};

struct App {
  App() {
    AudioPlayer player;
    player.clickPlay();
    player.clickLock();
    player.clickLock();
    player.clickNext();
    player.clickPrevious();
  }
};

int main() {
  App();
  return 0;
}

inline void LockedState::clickLock() {
  if (player.playing) {
    player.changeState(make_unique<PlayingState>(player));
  } else {
    player.changeState(make_unique<ReadyState>(player));
  }
}
inline void LockedState::clickPlay() {}
inline void LockedState::clickNext() {}
inline void LockedState::clickPrevious() {}

inline void ReadyState::clickLock() { player.changeState(make_unique<LockedState>(player)); }
inline void ReadyState::clickPlay() {
  player.startPlayback();
  player.changeState(make_unique<PlayingState>(player));
}
inline void ReadyState::clickNext() { player.nextSong(); }
inline void ReadyState::clickPrevious() { player.previousSong(); }

inline void PlayingState::clickLock() { player.changeState(make_unique<LockedState>(player)); }
inline void PlayingState::clickPlay() {
  player.stopPlayback();
  player.changeState(make_unique<ReadyState>(player));
}
inline void PlayingState::clickNext() { player.nextSong(); }
inline void PlayingState::clickPrevious() { player.previousSong(); }
