using DataBase;
using DataBase.DataObject;
using log4net;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using VAnalSvc.Properties;

namespace VAnalSvc
{
    class FrameAnalyzer
    {
        static ILog logger = LogManager.GetLogger(nameof(FrameAnalyzer));
        DBClient dbc;
        string video_id;
        int[] progressParts = new int[Settings.Default.Parallelism];
        int progressTotal = 0;

        public FrameAnalyzer(DBClient dbc)
        {
            this.dbc = dbc;
        }

        public void AnalyzeFrames(VideoInfo video)
        {
            video_id = video.VIDEO_ID;

            logger.Debug($"[AnalyzeFrames] start : VIDEO_ID[{video_id}]");

            dbc.UpdateVanalStatus_Start(video_id);

            double start = 0d;
            double end = video.PLAY_TIME;

            int parallelism = Settings.Default.Parallelism;
            double partTime = (end - start) / parallelism;

            logger.Debug($"[AnalyzeFrames] parallel info : parallelism[{parallelism}] partTime[{partTime}]");

            ManualResetEvent[] waitHandles = new ManualResetEvent[parallelism];
            for (int i = 0; i < parallelism; i++)
            {
                ManualResetEvent waitHandle = new ManualResetEvent(false);
                waitHandles[i] = waitHandle;

                int part = i + 1;
                double partStart = start + partTime * i;
                double partEnd = partStart + partTime;

                ThreadPool.QueueUserWorkItem(delegate 
                {
                    AnalyzePartial(video, part, partStart, partEnd);
                    waitHandle.Set();
                });
            }

            WaitHandle.WaitAll(waitHandles);

            logger.Debug($"[AnalyzeFrames] completed");
        }

        private void AnalyzePartial(VideoInfo video, int part, double start, double end)
        {
            logger.Error($"[AnalyzePartial_{part}] starting partial analyze : {TimeSpan.FromSeconds(start).ToString(@"hh\:mm\:ss\.fff")}~{TimeSpan.FromSeconds(end).ToString(@"hh\:mm\:ss\.fff")}");

            FrameGrabber frameGrabber = new FrameGrabber(video.FILE_PATH, start, end);
            if (frameGrabber.IsOpened() == false)
            {
                logger.Error($"[AnalyzePartial_{part}] failed to open video : {video.FILE_PATH}");
                return;
            }

            var frHelper = new FrHelper(new FrOption()
            {
                MinEyeRatio = video.MIN_EYE_RATIO,
                MaxEyeRatio = video.MAX_EYE_RATIO,
                Threshold = video.THRESHOLD
            });

            int lastProgress = 0;

            while (true)
            {
                try
                {
                    var frame = frameGrabber.GetNextFrame();
                    if (frame == null)
                        break;

                    double currentPositionSec = frameGrabber.CurrentPositionSec;
                    var result = frHelper.Analyze(frame, currentPositionSec);
                    if (result != null)
                    {
                        logger.Debug($"[AnalyzePartial_{part}] {result.Crop.Length} face found at {result.Snapshot.TimestampString}");
                        dbc.InsertAnalyzeResult(video_id, result);
                    }

                    int progress = frameGrabber.ProgressPercent(currentPositionSec);
                    if (lastProgress < progress)
                    {
                        lastProgress = progress;
                        logger.Debug($"[AnalyzePartial_{part}] progress : {progress}%");
                        ReportProgress(part, progress);
                    }

                    if (currentPositionSec >= end)
                        break;

                    if (frameGrabber.SkipFrame(video.SKIP_FRAME) >= end)
                        break;
                }
                catch (Exception ex)
                {
                    logger.Error($"[AnalyzePartial_{part}] {ex.Message}{Environment.NewLine}{ex.StackTrace}");
                }
            }
        }

        private void ReportProgress(int part, int progressPart)
        {
            progressParts[part - 1] = progressPart;
            int progress = (int)progressParts.Average();
            if (progressTotal < progress)
            {
                dbc.UpdateVanalStatus(video_id, Codes.VANAL_STS_CD.P, progress);
                logger.Debug($"[ReportProgress] progress : {progress}%");
            }
        }
    }
}
