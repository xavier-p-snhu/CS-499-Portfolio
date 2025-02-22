#include <stdio.h>  // for printf
#include <SQLAPI.h> // main SQLAPI++ header

int main(int argc, char* argv[])
{
    SAConnection con; // connection object
    SACommand cmd;    // command object
    
    try
    {
        // connect to database (Oracle in our example)
        con.Connect(_TSA("test"), _TSA("tester"), _TSA("tester"), SA_Oracle_Client);
        // associate a command with connection
        cmd.setConnection(&con);

        // Insert 2 rows
        cmd.setCommandText(
            _TSA("Insert into test_tbl(fid, fvarchar20) values(:1, :2)"));

        // use first method of binding - param assignment
        cmd.Param(1).setAsInt32() = 2;
        cmd.Param(2).setAsString() = _TSA("Some string (2)");
        // Insert first row
        cmd.Execute();

        // use second method of binding - stream binding
        cmd << 3 << _TSA("Some string (3)");
        // Insert second row
        cmd.Execute();

        // commit changes on success
        con.Commit();

        printf("Input parameters bound, rows inserted!\n");
    }
    catch(SAException &x)
    {
        // SAConnection::Rollback()
        // can also throw an exception
        // (if a network error for example),
        // we will be ready
        try
        {
            // on error rollback changes
            con.Rollback();
        }
        catch(SAException &)
        {
        }
        // print error message
        printf("%s\n", x.ErrText().GetMultiByteChars());
    }
    
    return 0;
}
