#ifndef SOUNDEFFECTS_H
#define SOUNDEFFECTS_H

// Libraries used for storing data
#include <QString>
#include <QUrl>

// Libraries used for audio playback
#include <QAudioOutput>
#include <QMediaPlayer>

class SoundPlayer {
public:
  QString fileLocation;
  void    playSoundEffect();
};

void SFXToPlay(QString filePath);

#endif // SOUNDEFFECTS_H
