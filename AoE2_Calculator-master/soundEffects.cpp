#include "SoundEffects.h"

void SoundPlayer::playSoundEffect()
{
  // Declaring what i'm using for SFX
  QAudioOutput* outPut = new QAudioOutput;
  QMediaPlayer* player = new QMediaPlayer;
  player->setAudioOutput(outPut);

  // Set the location of the media file
  player->setSource(QUrl::fromLocalFile(fileLocation));

  // Set the volume
  outPut->setVolume(100);

  // Play the media once
  player->setLoops(1);

  outPut->setMuted(false);

  // Play the media file
  player->play();
}




