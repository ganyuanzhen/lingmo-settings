#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QPointer>

class UpdateManager : public QObject {
  Q_OBJECT
 private:
  /**
   * @brief 是否有更新
   */
  bool has_updates_;

  std::shared_ptr<QNetworkAccessManager> m_manager;

 public:
  explicit UpdateManager(QObject* parent = nullptr);

  ~UpdateManager();

  /**
   * @brief 实现检查更新
   */
  Q_INVOKABLE void check_for_update();

  Q_INVOKABLE bool hasUpdate();

  Q_INVOKABLE void handle_update_data(QNetworkReply* reply);

 signals:
  void updateDataReply(QString data);

 public slots:
  void startCheckforUpdate();
};

#endif  // UPDATEMANAGER_H
