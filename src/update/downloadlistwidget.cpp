#include "downloadlistwidget.h"

#include <QDebug>
#include <QDesktopServices>
#include <QtConcurrent>

DownloadListWidget::DownloadListWidget(QObject *parent) : QObject(parent) {
  installEventFilter(this);
  // 计算显示下载速度
  download_speed.setInterval(1000);
  download_speed.start();
  connect(&download_speed, &QTimer::timeout, [=, this]() {
    if (isdownload && theSpeed == "") {
      size1 = download_size;
      double bspeed;
      bspeed = size1 - size2;
      if (bspeed < 1024) {
        theSpeed = QString::number(bspeed) + "B/s";
      } else if (bspeed < 1024 * 1024) {
        theSpeed = QString::number(0.01 * int(100 * (bspeed / 1024))) + "KB/s";
      } else if (bspeed < 1024 * 1024 * 1024) {
        theSpeed = QString::number(0.01 * int(100 * (bspeed / (1024 * 1024)))) +
                   "MB/s";
      } else {
        theSpeed =
            QString::number(0.01 * int(100 * (bspeed / (1024 * 1024 * 1024)))) +
            "GB/s";
      }
      size2 = download_size;
    }
    if (isdownload) {
      downloaditemlist[nowDownload - 1]->speed = theSpeed;
    } else {
      emit downloadProgress(0);
    }
  });
}

DownloadListWidget::~DownloadListWidget() {
  if (downloadController) {
    downloadController->disconnect();
    downloadController->stopDownload();
    // 这里没有释放 downloadController，使用懒汉式单例
  }

  clearItem();
}

bool DownloadListWidget::isDownloadInProcess() {
  if (toDownload > 0) {
    return true;
  }
  return false;
}

void DownloadListWidget::clearItem() {
  //  ui->listWidget->clear();
}

DownloadItem *DownloadListWidget::addItem(QString name, QString fileName,
                                          QString pkgName, QString downloadurl,
                                          int index) {
  if (fileName.isEmpty()) {
    return nullptr;
  }

  urList.append(downloadurl);
  allDownload += 1;
  toDownload += 1;

  DownloadItem *di = new DownloadItem(index, pkgName);
  connect(di, &DownloadItem::finished, this,
          &DownloadListWidget::slotInstallFinished, Qt::QueuedConnection);

  dlist << downloadurl;
  downloaditemlist << di;
  di->setFileName(fileName);
  di->setPkgName(index, pkgName);

  if (!isBusy) {
    nowDownload += 1;
    startRequest(urList.at(nowDownload - 1), fileName);  // 进行链接请求
  }

  return di;
}

QList<DownloadItem *> DownloadListWidget::getDIList() {
  return downloaditemlist;
}

QList<QString> DownloadListWidget::getUrlList() { return urList; }

void DownloadListWidget::startRequest(QString url, QString fileName) {
  isBusy = true;
  isdownload = true;
  downloaditemlist[allDownload - 1]->free = false;

  // 使用懒汉式单例来存储downloadController
  if (downloadController == nullptr) {
    downloadController =
        new DownloadController;  // 并发下载，在第一次点击下载按钮的时候才会初始化
  } else {
    downloadController->disconnect();
    downloadController->stopDownload();
  }

  connect(downloadController, &DownloadController::downloadProcess, this,
          &DownloadListWidget::updateDataReadProgress);
  connect(downloadController, &DownloadController::downloadFinished, this,
          &DownloadListWidget::httpFinished);
  connect(downloadController, &DownloadController::errorOccur, this,
          [=](QString msg) { qDebug() << msg; });
  downloadController->setFilename(fileName);
  downloadController->startDownload(url);
}

/***************************************************************
 *  @brief     下载列表完成下载的回调函数
 *  @param
 *  @note      如果正在安装，则在新开的线程空间中等待上一个安装完
 *  @Sample usage:
 **************************************************************/
void DownloadListWidget::httpFinished()  // 完成下载
{
  isdownload = false;
  isBusy = false;

  QtConcurrent::run([=, this]() {
    while (downloaditemlist[nowDownload - 1]->readyInstall() ==
           -1)  // 安装当前应用，堵塞安装，后面的下载suspend
    {
      QThread::msleep(500);  // 休眠500ms，减少CPU负担
      continue;
    }
    downloaditemlist[nowDownload - 1]->free = true;
    emit downloadFinished();

    if (nowDownload < allDownload) {
      // 如果有排队则下载下一个
      qDebug() << "Download: 切换下一个下载...";
      nowDownload += 1;
      while (nowDownload <= allDownload &&
             downloaditemlist[nowDownload - 1]->downloadCancelled_) {
        nowDownload += 1;
      }
      if (nowDownload <= allDownload) {
        QString fileName = downloaditemlist[nowDownload - 1]->getFileName();
        startRequest(urList.at(nowDownload - 1), fileName);
      }
    }
  });
}

void DownloadListWidget::updateDataReadProgress(QString speedInfo,
                                                qint64 bytesRead,
                                                qint64 totalBytes) {
  if (totalBytes <= 0) {
    return;
  }
  theSpeed = speedInfo;
  downloaditemlist[nowDownload - 1]->setMax(10000);  // 最大值
  downloaditemlist[nowDownload - 1]->setValue(
      int(bytesRead * 100 / totalBytes) * 100);  // 当前值
  emit downloadProgress(int(bytesRead * 100 / totalBytes));
  download_size = bytesRead;
  if (downloaditemlist[nowDownload - 1]->downloadCancelled_) {
    // 随时检测下载是否被取消
    downloadController->disconnect();
    downloadController->stopDownload();
    downloaditemlist[nowDownload - 1]->closeDownload();
    httpFinished();
  }
}

void DownloadListWidget::slotInstallFinished(bool success) {
  // NOTE: 仅在安装成功后判断是否需要退出后台
  if (!success) {
    qDebug() << "Download: install failed";
    return;
  }

  if (toDownload > 0) {
    toDownload -= 1;
    qDebug() << "Download: toDownload" << toDownload;
  }
}
