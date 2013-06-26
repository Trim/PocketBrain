Video with Game of Thrones generic and boxplots
-----------------------------------------------

Game of Emotions by PocketBrain project
---------------------------------------
Who will be master of his Emotions ?

Why to become 'master of Emotions' ?
--------------------------------

# If you have control of your emotions, you can
* keep calm in any sitation,
* improve the expression of what you want
* understand your feelings
* and you can influence others by deciding your feelings

# So, you can sit on the iron throne
Indeed, you will be "master of your emotions"
So you can take the best decisions in every situation
And you'll never be manipulated by others

# The question is now how can you learn to control your emotions ?
It can be done with help of the PocketBrain project

Indeed, The PocketBrain is a project to find emotions of people and to
sort them in different categories

# Why "to find emotions" ?
Because we have a lot of emotions which aren't still
easy to understand self and to express to other

# But our body already express our emotion state with mimics and other behaviours...
Indeed, but such behaviour can be controlled and modified by people
For example, actors have to express some emotion with their body behaviour,
but they don't feel in reality this emotion

# And what about to "classify" them in categories ?
Some research have been done last years to measure emotions of people
and they find two key concept to categorize emotions :
* The arousal : are you _calm_ or _exited_ ?
* The valence : feel you _positive_ or _negative_ ?

# show picture of emotions around cercle on axis

# How do you find these emotion keys ?
You have to use a computer and an EEG headset :
* Computer is used to compute these keys and categorize emotions
* EEG headset is used to measure the brain electric activity

# How to translate electric activity into arousal and valence ?
The computer have to _filter_ the measures of the electric activity to have the _alpha_ and _beta_ brain waves.
Then the computer make ratio of these waves between left and right hemisphere to get arousal and valence

# Then you can directly find emotion ?
No you have to train your computer with yourself, because arousal and valence is different for everyone.
* So it has to learn what is your _brain activity_ for each emotion you want to measure
* Then, when you want to find an emotion, it can apply statistics with current measure and the learned one to find how you feel

# Let's practice ! Hum..., computers aren't movable, so it's useless.
Indeed, it's better if you can measure emotions every where...
Hey, you can use your Android phone!
Indeed, smartphone are _real computers_ now !

# Fine ! But if you search "EEG" on Google, they are intrusive and unmovalbe tools
Indeed, but Emotiv have created light EEG headset to use as gamepad

# Enough words, have someone done it ?
Yes, SINLAB at EPFL (http://ldm.epfl.ch)

All the code is on github :
sinlabsemester-2013/PocketBrain :
 the application
sinlabsemester-2013/think-core :
fork of SBS2 (see below) with emokit inside

# Show me !
* capture application with Game of Thrones and explanations
* capture of PocketBrain and UI explanations : TRAIN
* capture of PocketBrain and UI explanations : TRY

Which are key pieces ?
----------------------
# Emocap headset
  * closed software (so encrypted communication)
# Emokit open-source project
  * decrypt data from headset
# SmartphoneBrainScanner2 project
  * provide a lot of tools to
   * get data from headset
   * apply some filters on them
   * make application for Android
# AOSP (open-source Android code)
 * To use your phone as you want
 
# What was your roadmap ?
1. Get SBS2 and compile it
2. Try to compile hello world applications
3. Make it work with headset
4. As expected we had to use Emokit and merge its code with SBS2
5. Run hello world and the SBS2 EEG viewer
7. Try to run them on Android Emulator
    Found that emulator can't reach USB devices and so code on desktop
8. Try to use the 3D Brain application
    We had several bugs to run it and we've tried to fix them
    But we weren't able and we asked help to main developpers but
    we learned that it was a proof of concept not totally finished
9. Create our BrainPocket applications
10. Read colorOfMind documentation and apply it in our application
    We had to hack SBS2 to apply two filters in same time
11. Find NaiveBayes documentation and apply it
11. Make tests and fix bugs
12. Run in real situation with a subject


# Special thanks to
* Alex Barchiesi
* Carlos Sanchez
* Florine Dorsaz
* Emokit, SmartphoneBrainScanner2
* KDEnlive, GNU R, Qt
* and many others
