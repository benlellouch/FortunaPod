# FortunaPod

<img src="images/IMG_3205.jpg" width="500" >

## What is it?
FortunaPod is a music player for La Ruota della Fortuna board which is powered by an ATMEL AT90USB1286. It provides a simple, responsive and intuitive UI for the user to scroll through and play the songs he has written to his micro sd-card.

## What type of audio can it play?
It plays audio in the WAV format. Due to the limitations of the processor it can only play back smooth audio at 8KHz in stereo or 16KHz in mono (I would recommend using [this](https://audio.online-convert.com/convert-to-wav) website to convert you audio to WAV as it provides an easy to change bit resolution, sample rate and audio channels).

## Libraries I have used
+ [lafortuna-wav-lib](https://github.com/fatcookies/lafortuna-wav-lib)
+ [FatFs](http://elm-chan.org/fsw/ff/00index_e.html)
