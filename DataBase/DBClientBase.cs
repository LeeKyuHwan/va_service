using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataBase
{
    public class DBClientBase : IDisposable
    {
        const string connStrFormat = @"
User Id={0}; Password={1}; 
Data Source= 
(DESCRIPTION=
    (ADDRESS=
      (PROTOCOL=TCP)
      (HOST={2})
      (PORT={3})
    )
    (CONNECT_DATA=
      (SERVER=dedicated)
      (SERVICE_NAME={4})
    )
  )";

        protected OracleConnection conn;
        bool disposed = false;

        public DBClientBase(string connStr)
        {
            connect(connStr);
        }

        private void connect(string connStr)
        {
            conn = new OracleConnection();
            conn.ConnectionString = connStr;
            conn.Open();
        }

        #region Dispose
        ~DBClientBase()
        {
            Dispose(false);
        }

        #region IDisposable 멤버

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        #endregion

        private void Dispose(bool disposing)
        {
            if (!this.disposed)
            {
                if (disposing)
                {
                    if (conn != null)
                    {
                        conn.Close();
                        conn.Dispose();
                        conn = null;
                    }
                }
            }
            this.disposed = true;
        }
        #endregion

        protected static string CreateConnectionStr(string user, string password, string host, int port, string serviceName)
        {
            return string.Format(connStrFormat, user, password, host, port, serviceName).Replace("\r\n", string.Empty);
        }

        public void InsertData(DataTable dt)
        {
            string sql = createInsertStatement(dt);

            using (OracleCommand cmd = new OracleCommand(sql, conn))
            {
                cmd.BindByName = true;

                for (int i = 0; i < dt.Rows.Count; i++)
                    insertRow(cmd, dt.Rows[i]);
            }
        }

        string createInsertStatement(DataTable dt)
        {
            StringBuilder sb = new StringBuilder();
            sb.AppendFormat("INSERT INTO {0} (", dt.TableName);

            for (int i = 0; i < dt.Columns.Count; i++)
                sb.AppendFormat("{0},", dt.Columns[i].ColumnName);

            sb.Remove(sb.Length - 1, 1);
            sb.Append(") VALUES (");

            for (int i = 0; i < dt.Columns.Count; i++)
                sb.AppendFormat(":v{0},", i);

            sb.Remove(sb.Length - 1, 1);
            sb.Append(")");

            return sb.ToString();
        }

        int insertRow(OracleCommand cmd, DataRow row)
        {
            cmd.Parameters.Clear();

            for (int i = 0; i < row.Table.Columns.Count; i++)
                cmd.Parameters.Add("v" + i.ToString(), row[i]);

            return cmd.ExecuteNonQuery();
        }

        public string[] GetColumns(string tableName)
        {
            string sql = @"
SELECT COLUMN_NAME FROM USER_TAB_COLUMNS
WHERE TABLE_NAME = :I_TABLE_NAME
";
            using (OracleDataAdapter oda = new OracleDataAdapter(sql, conn))
            {
                oda.SelectCommand.Parameters.Add("I_TABLE_NAME", tableName);

                DataTable dt = new DataTable();
                oda.Fill(dt);

                List<string> tables = new List<string>(dt.Rows.Count);
                for (int i = 0; i < dt.Rows.Count; i++)
                    tables.Add(dt.Rows[i][0].ToString());

                return tables.ToArray();
            }
        }

        OracleParameter createParam<T>(string name, T value, OracleDbType dbType)
        {
            OracleParameter param = new OracleParameter(name, dbType);
            param.Direction = ParameterDirection.Input;
            param.Value = value;

            return param;
        }

        OracleParameter createOParam<T>(string name, OracleDbType dbType, int size)
        {
            OracleParameter param = new OracleParameter(name, dbType);
            param.Direction = ParameterDirection.Output;
            param.Size = size;

            return param;
        }

        protected OracleParameter CreateParam(string name, string value)
        {
            return createParam<string>(name, value, OracleDbType.Varchar2);
        }

        protected OracleParameter CreateParam(string name, int value)
        {
            return createParam<int>(name, value, OracleDbType.Int32);
        }

        protected OracleParameter CreateParam(string name, float value)
        {
            return createParam<float>(name, value, OracleDbType.Single);
        }

        protected OracleParameter CreateParam(string name, double value)
        {
            return createParam<double>(name, value, OracleDbType.Double);
        }

        protected OracleParameter CreateParam(string name, byte[] value)
        {
            return createParam<byte[]>(name, value, OracleDbType.Blob);
        }

        protected OracleParameter CreateOParam(string name, int size)
        {
            return createOParam<string>(name, OracleDbType.Varchar2, size);
        }

        protected DataTable Fill(string sql, params OracleParameter[] paramList)
        {
            using (OracleDataAdapter oda = new OracleDataAdapter(sql, conn))
            {
                oda.SelectCommand.BindByName = true;
                if (paramList.Length > 0)
                    oda.SelectCommand.Parameters.AddRange(paramList);

                DataTable dt = new DataTable();
                oda.Fill(dt);

                return dt;
            }
        }

        protected T[] QueryArray<T>(string sql, string column, params OracleParameter[] paramList)
        {
            DataTable dt = Fill(sql, paramList);
            return dt.AsEnumerable().Select(x => x.Field<T>(column)).ToArray();
        }

        protected int ExecuteNonQuery(string sql, params OracleParameter[] paramList)
        {
            using (OracleCommand cmd = new OracleCommand(sql, conn))
            {
                cmd.BindByName = true;
                if (paramList.Length > 0)
                    cmd.Parameters.AddRange(paramList);

                return cmd.ExecuteNonQuery();
            }
        }

        protected int ExecuteNonQuery(string sql, int arrayBindCount, params OracleParameter[] paramList)
        {
            using (OracleCommand cmd = new OracleCommand(sql, conn))
            {
                cmd.BindByName = true;
                cmd.ArrayBindCount = arrayBindCount;

                if (paramList.Length > 0)
                    cmd.Parameters.AddRange(paramList);

                return cmd.ExecuteNonQuery();
            }
        }

        protected object ExecuteScalar(string sql, params OracleParameter[] paramList)
        {
            using (OracleCommand cmd = new OracleCommand(sql, conn))
            {
                cmd.BindByName = true;
                if (paramList.Length > 0)
                    cmd.Parameters.AddRange(paramList);

                return cmd.ExecuteScalar();
            }
        }

        protected object ExecuteScalar(string sql, int arrayBindCount,params OracleParameter[] paramList)
        {
            using (OracleCommand cmd = new OracleCommand(sql, conn))
            {
                cmd.BindByName = true;
                cmd.ArrayBindCount = arrayBindCount;

                if (paramList.Length > 0)
                    cmd.Parameters.AddRange(paramList);

                return cmd.ExecuteScalar();
            }
        }

        protected T[] Pivot<T>(DataTable dt, string column)
        {
            return (from row in dt.AsEnumerable()
                    select row.Field<T>(column)).ToArray();
        }

        public string GetUser()
        {
            string sql = "SELECT USER FROM DUAL";
            return ExecuteScalar(sql).ToString();
        }

        public OracleCommand CreateCommand()
        {
            OracleCommand cmd = new OracleCommand();
            cmd.Connection = conn;
            return cmd;
        }

        public void PrintScheme(DataTable dt)
        {
            for (int i = 0; i < dt.Columns.Count; i++)
                Debug.WriteLine("[{0}] {1}", i, dt.Columns[i].ColumnName);
        }

        public void Procecure(string name, OracleParameter[] inPrarams, OracleParameter[] outPrarams)
        {
            using (OracleCommand cmd = new OracleCommand(name, conn))
            {
                cmd.CommandType = CommandType.StoredProcedure;
                cmd.BindByName = true;

                if (inPrarams != null && inPrarams.Length > 0)
                    cmd.Parameters.AddRange(inPrarams);

                if (outPrarams != null && outPrarams.Length > 0)
                {
                    for (int i = 0; i < outPrarams.Length; i++)
                        outPrarams[i].Direction = ParameterDirection.Output;
                    cmd.Parameters.AddRange(outPrarams);
                }

                cmd.ExecuteNonQuery();
            }
        }
    }
}
