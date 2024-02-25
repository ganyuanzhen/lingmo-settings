#ifndef DOWNLOADLISTWIDGET_H
#define DOWNLOADLISTWIDGET_H

#include <QObject>
#include <QTimer>

#include "download_item.h"
#include "downloadworker.h"

class DownloadListWidget : public QObject {
  Q_OBJECT

 public:
  DownloadItem *addItem(QString name, QString fileName, QString pkgName,
                        QString downloadurl, int index);
  int nowDownload = 0;
  int allDownload = 0;
  int toDownload = 0;
  QList<DownloadItem *> getDIList();
  QList<QString> getUrlList();
  void m_move(int x, int y);
  explicit DownloadListWidget(QObject *parent = nullptr);
  ~DownloadListWidget() override;

  bool isDownloadInProcess();

 private:
  int isdownload = false;
  bool isBusy = false;
  QStringList dlist;
  QList<QString> urList;
  QList<DownloadItem *> downloaditemlist;
  DownloadController *downloadController = nullptr;
  int nowdownload = 0;
  QString theSpeed;
  QTimer download_speed;
  long download_size = 0;
  long size1 = 0;
  long size2 = 0;
  void startRequest(QString url, QString fileName);
  void httpFinished();
  void updateDataReadProgress(QString speedInfo, qint64 bytesRead,
                              qint64 totalBytes);
  void clearItem();

 signals:
  void downloadFinished();
  void downloadProgress(int i);

 private slots:
  void slotInstallFinished(bool success);
};

#endif  // DOWNLOADLISTWIDGET_H
