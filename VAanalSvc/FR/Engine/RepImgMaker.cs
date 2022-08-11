using DataBase;
using DataBase.DataObject;
using log4net;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace VAnalSvc
{
    class RepImgMaker
    {
        static ILog logger = LogManager.GetLogger(nameof(RepImgMaker));

        DBClient dbc;
        FrHelper frHelper = new FrHelper();
        const float threshold = 0.9f;

        public RepImgMaker(DBClient dbc)
        {
            this.dbc = dbc;
        }

        public void SetRepresentation(VideoInfo video)
        {
            string video_id = video.VIDEO_ID;

            logger.Debug($"[SetRepresentation] start : video_id[{video_id}]");

            dbc.UpdateVanalStatus(video_id, Codes.VANAL_STS_CD.M, 100);

            logger.Debug($"[SetRepresentation] verifying...");
            var dt = dbc.GetCropResult(video_id);
            dt = SetRepresentation(dt);

            logger.Debug($"[SetRepresentation] updating p_crop_id");
            dbc.UpdatePCropId(video_id, dt);

            logger.Debug($"[SetRepresentation] updating representation by quality score");
            dbc.SetRepresentation(video_id);    

            logger.Debug($"[SetRepresentation] completed : video_id[{video_id}]");
        }

        DataTable SetRepresentation(DataTable dt)
        {
            if (dt.Rows.Count == 0)
            {
                return dt;
            }
            else if (dt.Rows.Count == 1)
            {
                dt.Rows[0]["P_CROP_ID"] = dt.Rows[0]["CROP_ID"];
                return dt;
            }

            for (int i = 0; i < dt.Rows.Count; i++)
            {
                DataRow row = dt.Rows[i];

                var snapshot_time = Convert.ToSingle(row["SNAPSHOT_TIME"]);
                var snapshot_id = row.Field<string>("SNAPSHOT_ID");

                DataRow[] rows = GetTarget(dt, i + 1, snapshot_time, snapshot_id);
                if (rows.Length == 0)
                    continue;

                var crop_id = row.Field<string>("CROP_ID");
                using (var fir = frHelper.BuildFir((byte[])row["FIR"]))
                    SetRepresentationPart(fir, crop_id, rows);
            }

            var aa = (from r in dt.AsEnumerable()
                      where string.IsNullOrEmpty(r.Field<string>("P_CROP_ID"))
                      select r).ToArray();

            return dt;
        }

        DataRow[] GetTarget(DataTable dt, int startIdx, float snapshot_time, string snapshot_id)
        {
            List<DataRow> rows = new List<DataRow>();

            for (int i = startIdx; i < dt.Rows.Count; i++)
            {
                DataRow row = dt.Rows[i];

                // 1초 이내에 다시 출현한 얼굴에 대해서만 처리
                if (Convert.ToSingle(row["SNAPSHOT_TIME"]) >= snapshot_time + 1)
                    break;

                // 같은 스냅샷에 포함되지 않은 얼굴만 처리
                if (row.Field<string>("SNAPSHOT_ID").Equals(snapshot_id))
                    continue;

                // 동일인물이 결정되지 않은 얼굴만 처리
                if (string.IsNullOrEmpty(row.Field<string>("P_CROP_ID")) == false)
                    continue;

                rows.Add(row);
            }

            return rows.ToArray();
        }

        void SetRepresentationPart(Cognitec.FRsdk.FIR fir, string p_crop_id, DataRow[] rows)
        {
            for (int i = 0; i < rows.Length; i++)
            {
                DataRow row = rows[i];

                using (var target = frHelper.BuildFir((byte[])row["FIR"]))
                {
                    float score = frHelper.Compare(fir, target);
                    if (score > threshold)
                    {
                        row["P_CROP_ID"] = p_crop_id;
                        return;
                    }
                }
            }
        }
    }
}
