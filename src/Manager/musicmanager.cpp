#include "musicmanager.h"
#include <qdebug.h>

MusicManager* MusicManager::m_instance = nullptr;

MusicManager::MusicManager(QObject *parent) : QObject(parent)
{
    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_player->setAudioOutput(m_audioOutput);

    connect(m_player, &QMediaPlayer::playbackStateChanged, this, [=](QMediaPlayer::PlaybackState state) {
        if (state == QMediaPlayer::StoppedState && !m_currentUrl.isEmpty()) {
            onPlaybackEnded();
        }
    });

    setVolume(50);
}

MusicManager* MusicManager::getInstance()
{
    if (m_instance == nullptr)
    {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (m_instance == nullptr)
        {
            m_instance = new MusicManager();
        }
    }
    return m_instance;
}

void MusicManager::playMusic(const QString& musicPath)
{
    m_currentUrl = QUrl("qrc" + musicPath);
    m_player->setSource(m_currentUrl);
    m_player->play();
}



void MusicManager::onPlaybackEnded()
{
    m_player->setSource(m_currentUrl);
    m_player->play();
}

void MusicManager::pauseMusic()
{
    if (m_player->playbackState() == QMediaPlayer::PlayingState)
        m_player->pause();
}

void MusicManager::resumeMusic()
{
    if (m_player->playbackState() == QMediaPlayer::PausedState)
        m_player->play();
}

void MusicManager::stopMusic()
{
    m_player->stop();
    m_currentUrl.clear();
}

void MusicManager::setVolume(int volume)
{
    m_audioOutput->setVolume(qBound(0, volume, 100) / 100.0);
}
