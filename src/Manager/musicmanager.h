#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include <QObject>
#include <QMediaPlayer>
#include <QUrl>
#include <QAudioOutput>
#include <QMutex>

class MusicManager : public QObject
{
    Q_OBJECT
private:
    explicit MusicManager(QObject *parent = nullptr);
    ~MusicManager() = default;

    MusicManager(const MusicManager&) = delete;
    MusicManager& operator=(const MusicManager&) = delete;

private slots:
    void onPlaybackEnded();

public:
    static MusicManager* getInstance();
    void playMusic(const QString& musicPath);
    void pauseMusic();
    void resumeMusic();
    void stopMusic();
    void setVolume(int volume);

private:
    QMediaPlayer* m_player;
    QAudioOutput* m_audioOutput;
    QUrl m_currentUrl;
    static MusicManager* m_instance;
};

#endif // MUSICMANAGER_H
