using System;
using System.Data;
using MySql.Data.MySqlClient;

public class Test
{
   public static void Main(string[] args)
   {
      string connectionString =
        "Server=localhost;" +
         "Database=hashmodel;" +
         "User ID=usuario;" +
         "Password=senha;" +
         "Pooling=false";

      IDbConnection dbcon;
      dbcon = new MySqlConnection(connectionString);
      dbcon.Open();
      IDbCommand dbcmd = dbcon.CreateCommand();
      string sql =  "show databases";

      dbcmd.CommandText = sql;
      IDataReader reader = dbcmd.ExecuteReader();
      while(reader.Read()) {
         Console.WriteLine(reader.GetString(0));
      }

      reader.Close();
      reader = null;
      dbcmd.Dispose();
      dbcmd = null;
      dbcon.Close();
      dbcon = null;
   }
}
