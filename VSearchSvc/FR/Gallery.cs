using log4net;
using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace VSearchSvc
{
    class Gallery
    {
        static ILog logger = LogManager.GetLogger(nameof(Gallery));

        ReaderWriterLockSlim rwLock = new ReaderWriterLockSlim();
        Dictionary<string, List<Item>> Videos = new Dictionary<string, List<Item>>();

        public int Count
        {
            get
            {
                rwLock.EnterReadLock();
                int count = Videos.Sum(x => x.Value.Count);
                rwLock.ExitReadLock();

                return count;
            }
        }

        public int AddItem(string video_id, string key, byte[] fir)
        {
            var item = new Item() { Key = key, FIR = fir };

            try
            {
                rwLock.EnterWriteLock();

                if (Videos.ContainsKey(video_id) == false)
                {
                    Videos.Add(video_id, new List<Item>(new Item[1] { item }));
                }
                else
                {
                    Videos[video_id].Add(item);
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[Add] {item} : {ex.Message}");
            }
            finally
            {
                rwLock.ExitWriteLock();
            }

            return Count;
        }

        public int AddRange(string video_id, Item[] items)
        {
            try
            {
                rwLock.EnterWriteLock();

                if (Videos.ContainsKey(video_id) == false)
                {
                    Videos.Add(video_id, new List<Item>(items));
                }
                else
                {
                    Videos[video_id].AddRange(items);
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[AddRange] {ex.Message}");
            }
            finally
            {
                rwLock.ExitWriteLock();
            }

            return Count;
        }

        public int Remove(string video_id, string key)
        {
            try
            {
                rwLock.EnterWriteLock();

                if (Videos.TryGetValue(video_id, out var items))
                {
                    int idx = items.FindIndex(x => x.Key.Equals(key));
                    if (idx >= 0)
                    {
                        items.RemoveAt(idx);
                    }
                    else
                    {
                        logger.Error($"[Remove] key not found : {key}");
                    }
                }
                else
                {
                    logger.Error($"video id not found : {video_id}");
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[Remove] key[{key}] : {ex.Message}");
            }
            finally
            {
                rwLock.ExitWriteLock();
            }

            return Count;
        }

        public int RemoveVideo(string video_id)
        {
            try
            {
                rwLock.EnterWriteLock();

                if (Videos.Remove(video_id) == false)
                {
                    logger.Error($"[RemoveVideo] video_id not found : {video_id}");
                }
            }
            catch (Exception ex)
            {
                logger.Error($"[RemoveVideo] video_id[{video_id}] : {ex.Message}");
            }
            finally
            {
                rwLock.ExitWriteLock();
            }

            return Count;
        }

        public Cognitec.FRsdk.Match[] Matches(string video_id, Cognitec.FRsdk.FIR fir, float threshold)
        {
            var matches = new List<Cognitec.FRsdk.Match>();

            try
            {
                rwLock.EnterReadLock();

                if (Videos.TryGetValue(video_id, out var items))
                {
                    for (int i = 0; i < items.Count; i++)
                    {
                        float score = items[i].Compare(fir);
                        if (score >= threshold)
                        {
                            matches.Add(new Cognitec.FRsdk.Match()
                            {
                                name = items[i].Key,
                                score = new Cognitec.FRsdk.Score() { value = score }
                            });
                        }
                    }

                    matches.Sort(sortByScoreDesc);
                }

                logger.Debug($"[Matches] {matches.Count} matches");
            }
            catch (Exception ex)
            {
                logger.Error($"[Matches] {ex.Message}");
            }
            finally
            {
                rwLock.ExitReadLock();
            }

            return matches.ToArray();
        }

        private int sortByScoreDesc(Cognitec.FRsdk.Match m1, Cognitec.FRsdk.Match m2)
        {
            return m2.score.value.CompareTo(m1.score.value);
        }
    }

    class VideoMatchResult
    {
        public readonly string VIDEO_ID;
        public readonly string[] IMAGE_ID;
        public readonly float[] SCORE;

        public VideoMatchResult(string video_id, string[] image_id, float[] score)
        {
            VIDEO_ID = video_id;
            IMAGE_ID = image_id;
            SCORE = score;
        }
    }

    class Item
    {
        public string Key;
        public byte[] FIR;

        public static Item FromDataRow(DataRow row)
        {
            return new Item()
            {
                Key = row.Field<string>(0),
                FIR = (byte[])row[1]
            };
        }

        public static Item[] FromDataTable(DataTable dt)
        {
            Item[] items = new Item[dt.Rows.Count];

            for (int i = 0; i < dt.Rows.Count; i++)
            {
                items[i] = new Item()
                {
                    Key = dt.Rows[i].Field<string>(0),
                    FIR = (byte[])dt.Rows[i][1]
                };
            }

            return items;
        }

        public float Compare(Cognitec.FRsdk.FIR fir)
        {
            return FrHelper.Instance.Compare(fir, FIR);
        }

        public override string ToString()
        {
            return $"key[{Key}] fir[{FIR.Length}bytes]";
        }
    }
}
