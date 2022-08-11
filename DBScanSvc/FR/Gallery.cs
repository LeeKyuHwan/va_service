using log4net;
using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace DBScanSvc
{
    class Gallery
    {
        static ILog logger = LogManager.GetLogger(nameof(Gallery));

        ReaderWriterLockSlim rwLock = new ReaderWriterLockSlim();
        List<Item> Items = new List<Item>();
        public int Count => Items.Count;

        public int Add(string key, byte[] fir)
        {
            var item = new Item() { Key = key, FIR = fir };
            var count = 0;

            try
            {
                rwLock.EnterWriteLock();

                Items.Add(item);
                count = Count;

                logger.Debug($"[Add] total count : {count}");
            }
            catch (Exception ex)
            {
                logger.Error($"[Add] {item} : {ex.Message}");
            }
            finally
            {
                rwLock.ExitWriteLock();
            }

            return count;
        }

        public int AddRange(Item[] items)
        {
            var count = 0;

            try
            {
                rwLock.EnterWriteLock();

                Items.AddRange(items);
                count = Count;

                logger.Debug($"[AddRange] {items.Length} added, total count : {count}");
            }
            catch (Exception ex)
            {
                logger.Error($"[AddRange] {ex.Message}");
            }
            finally
            {
                rwLock.ExitWriteLock();
            }

            return count;
        }

        public int Remove(string key)
        {
            var count = 0;

            try
            {
                rwLock.EnterWriteLock();

                int idx = Items.FindIndex(x => x.Key.Equals(key));
                if (idx >= 0)
                {
                    Items.RemoveAt(idx);
                    count = Count;
                    logger.Debug($"[Remove] total count : {count}");
                }
                else
                {
                    logger.Error($"[Remove] key not found : {key}");
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

            return count;
        }

        public Cognitec.FRsdk.Match[] Matches(Cognitec.FRsdk.FIR fir, float threshold)
        {
            var matches = new List<Cognitec.FRsdk.Match>();

            try
            {
                rwLock.EnterReadLock();

                for (int i = 0; i < Items.Count; i++)
                {
                    float score = Items[i].Compare(fir);
                    if (score >= threshold)
                    {
                        matches.Add(new Cognitec.FRsdk.Match()
                        {
                            name = Items[i].Key,
                            score = new Cognitec.FRsdk.Score() { value = score }
                        });
                    }
                }

                matches.Sort(sortByScoreDesc);

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
