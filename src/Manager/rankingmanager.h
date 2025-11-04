#ifndef RANKINGMANAGER_H
#define RANKINGMANAGER_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDateTime>
#include <QCoreApplication>

// 排行榜单条记录结构体
struct RankingRecord {
    int score;          // 分数
    QString time;       // 记录时间

    bool operator==(const RankingRecord& other) const {
        return score == other.score && time == other.time;
    }
};

class RankingManager : public QObject {
    Q_OBJECT
private:
    // 单例模式：唯一实例
    explicit RankingManager(QObject *parent = nullptr);
    ~RankingManager() = default;

public:
    // 获取单例实例
    static RankingManager* getInstance();

    // 加载排行榜（从JSON文件）
    void loadRanking();

    // 保存排行榜（到JSON文件）
    void saveRanking();

    // 更新排行榜（传入当前分数，返回是否进入榜单）
    bool updateRanking(int currentScore);

    // 获取排行榜数据（按分数降序）
    QVector<RankingRecord> getRankingList() const;

private:
    // 检查本地文件，若不存在则从资源复制
    void checkAndCopyResourceFile();
    QVector<RankingRecord> m_rankingList;
    const QString m_resourcePath = ":/data/ranking.json"; // 资源文件路径（qrc路径）
    const QString m_localPath = QCoreApplication::applicationDirPath() + "/ranking.json"; // 本地文件路径（程序运行目录）
    const int m_maxRecordCount = 10;
};

#endif // RANKINGMANAGER_H
