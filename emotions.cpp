#include "emotions.h"

Emotions::Emotions()
{
    arousal_max=UNSET_VALUE;
    arousal_min=UNSET_VALUE;

    valence_max=UNSET_VALUE;
    valence_min=UNSET_VALUE;

    max_diff_arousal=0;
    max_diff_valence=0;
    resetValues();
}

void Emotions::resetValues(){
    arousal=UNSET_VALUE;
    arousal_4s=UNSET_VALUE;
    arousal_20s=UNSET_VALUE;
    arousal_wait_4s=true;
    arousal_wait_20s=true;

    valence=UNSET_VALUE;
    valence_4s=UNSET_VALUE;
    valence_20s=UNSET_VALUE;
    valence_wait_4s=true;
    valence_wait_20s=true;
}

void Emotions::wait_arousal_4s(){
    arousal_wait_4s=false;
}
void Emotions::wait_arousal_20s(){
    arousal_wait_20s=false;
}
void Emotions::wait_valence_4s(){
    valence_wait_4s=false;
}
void Emotions::wait_valence_20s(){
    valence_wait_20s=false;
}

void Emotions::compute_arousal(double value){
    if(arousal_wait_4s){
        QTimer::singleShot(4000, this, SLOT(wait_arousal_4s()));
    }else{
        if(arousal_4s==UNSET_VALUE)
            arousal_4s=value;
    }

    if(arousal_wait_20s){
        QTimer::singleShot(20000, this, SLOT(wait_arousal_20s()));
    }else{
        if(arousal_20s==UNSET_VALUE)
            arousal_20s=value;
    }

    if(arousal_4s!=-1 && arousal_20s!=-1){
        double arousal_diff = abs(arousal_4s-arousal_20s);
        if(max_diff_arousal < arousal_diff)
            max_diff_arousal = arousal_diff;
        arousal=(arousal_4s-arousal_20s)/max_diff_arousal;

        if(arousal_max<arousal || arousal_max==UNSET_VALUE)
            arousal_max=arousal;
        if(arousal_min>arousal || arousal_max==UNSET_VALUE)
            arousal_min=arousal;

        computeEmotion();
    }
}

void Emotions::compute_valence(double value){
    if(valence_wait_4s){
        QTimer::singleShot(4000, this, SLOT(wait_valence_4s()));
    }else{
        if(valence_4s==UNSET_VALUE)
            valence_4s=value;
    }

    if(valence_wait_20s){
        QTimer::singleShot(20000, this, SLOT(wait_valence_20s()));
    }else{
        if(valence_20s==UNSET_VALUE)
            valence_20s=value;
    }

    if(valence_4s!=-1 && valence_20s!=-1){
        double valence_diff = abs(valence_4s-valence_20s);
        if(max_diff_valence < valence_diff)
            max_diff_valence = valence_diff;
        valence=(valence_4s-valence_20s)/max_diff_valence;

        if(valence_max<valence || arousal_min==UNSET_VALUE)
            valence_max=valence;
        if(valence_min>valence || arousal_min==UNSET_VALUE)
            valence_min=valence;

        computeEmotion();
    }
}

void Emotions::computeEmotion(){
    QString emotion = *(new QString("error_novalue"));

    //TODO : use better algorithm to suppress too high/min value
    double arousal_center = (arousal_max+arousal_min)/2;
    double arousal_mean_exited = arousal_max/2;
    double arousal_mean_calm = arousal_min/2;

    double valence_center = (valence_max+valence_min)/2;
    double valence_mean_positive = valence_max/2;
    double valence_mean_negative = valence_min/2;

    if(valence!=UNSET_VALUE && arousal!=UNSET_VALUE){
        if(arousal >= arousal_center
                 && valence >= valence_center){
            emotion="calm";
        }else if(arousal < arousal_center
                 && valence < valence_center){
            emotion="sad";
        }else if((arousal > arousal_center && arousal <= arousal_mean_exited)
                 && valence < valence_center){
            emotion="fear";
        }else if((arousal > arousal_mean_exited)
                 && valence < valence_center){
            emotion="angry";
        }else if((arousal > arousal_center && arousal <= arousal_mean_exited)
                 && valence > valence_center){
            emotion="joy";
        }else if((arousal>arousal_mean_exited)
                 && valence > valence_center){
            emotion="surprise";
        }

        emit giveEmotion(emotion);
        resetValues();
    }
}
