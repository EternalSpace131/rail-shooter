#include "rankingmanager.h"
#include <QJsonDocument>
#include <QDebug>
#include <QFileInfo>

RankingManager* RankingManager::getInstance() {
    static RankingManager instance;
    return &instance;
}

RankingManager::RankingManager(QObject *parent) : QObject(parent) {
    checkAndCopyResourceFile(); // 启动时检查并复制资源文件
    loadRanking(); // 加载本地文件
}

// 关键：检查本地文件，不存在则从资源复制到本地
void RankingManager::checkAndCopyResourceFile() {
    QFileInfo localFileInfo(m_localPath);
    if (localFileInfo.exists()) {
        qDebug() << "本地排行榜文件已存在：" << m_localPath;
        return;
    }

    // 本地文件不存在，从资源文件复制
    QFile resourceFile(m_resourcePath);
    QFile localFile(m_localPath);

    // 打开资源文件（只读）
    if (!resourceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "资源文件打开失败：" << resourceFile.errorString();
        // 若资源文件也不存在，创建空的本地文件
        if (localFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            localFile.write("[]"); // 空JSON数组
            localFile.close();
        }
        return;
    }

    // 打开本地文件（写入）
    if (!localFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qDebug() << "本地文件创建失败：" << localFile.errorString();
        resourceFile.close();
        return;
    }

    // 复制资源文件内容到本地
    QByteArray data = resourceFile.readAll();
    localFile.write(data);

    // 关闭文件
    resourceFile.close();
    localFile.close();
    qDebug() << "从资源复制排行榜文件到本地：" << m_localPath;
}

// 加载排行榜：读取本地文件
void RankingManager::loadRanking() {
    QFile file(m_localPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "本地排行榜文件打开失败：" << file.errorString();
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    file.close();

    if (!doc.isArray()) {
        qDebug() << "排行榜文件格式错误，重置为空榜单";
        m_rankingList.clear();
        saveRanking(); // 重置为空文件
        return;
    }

    QJsonArray jsonArray = doc.array();
    m_rankingList.clear();
    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonObject obj = jsonArray[i].toObject();
        RankingRecord record;
        record.score = obj["score"].toInt();
        record.time = obj["time"].toString();
        m_rankingList.append(record);
    }

    // 按分数降序排序
    std::sort(m_rankingList.begin(), m_rankingList.end(), [](const RankingRecord& a, const RankingRecord& b) {
        return a.score > b.score;
    });
}

// 保存排行榜：写入本地文件
void RankingManager::saveRanking() {
    QJsonArray jsonArray;
    for (const auto& record : m_rankingList) {
        QJsonObject obj;
        obj["score"] = record.score;
        obj["time"] = record.time;
        jsonArray.append(obj);
    }

    QJsonDocument doc(jsonArray);
    QFile file(m_localPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qDebug() << "排行榜文件保存失败：" << file.errorString();
        return;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    qDebug() << "保存成功";
}

// 更新排行榜
bool RankingManager::updateRanking(int currentScore) {
    if (currentScore <= 0) return false;

    RankingRecord newRecord;
    newRecord.score = currentScore;
    newRecord.time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    m_rankingList.append(newRecord);
    std::sort(m_rankingList.begin(), m_rankingList.end(), [](const RankingRecord& a, const RankingRecord& b) {
        return a.score > b.score;
    });

    if (m_rankingList.size() > m_maxRecordCount) {
        m_rankingList.resize(m_maxRecordCount);
    }

    saveRanking();
    return std::find(m_rankingList.begin(), m_rankingList.end(), newRecord) != m_rankingList.end();
}

QVector<RankingRecord> RankingManager::getRankingList() const {
    return m_rankingList;
}
