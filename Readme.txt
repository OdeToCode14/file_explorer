  File Explorer
Developed by :Amit Tiwari
    Roll No. :2018201099

Application can be compiled with 'make' . And the application can be started with ./console 

Available modes

Normal Mode:
keys and their behaviour:
     enter key : to diplay contents of a folder or to a open some particular files
      colon(:) : to switch to command mode.
             h : to move to home directory i.e. the folder where application was started
             q : to exit the application.
left arrow key : to move to previously visited folder
right arrow key: to move to folder visited after current folder.
 up arrow key  : to move up in the traversing list.
down arrow key : to move down in the traversing list.


Command Mode:
In this mode desired operations are performed with commands. In case the command format is wrong or arguments passed or wrong or the invalid path is passed, an invalid status is prompted.

keys and their behaviour:
some alphanumeric characters and special characters : for command input
                                         Escape key : for switching to normal mode
                                      Backspace key : for deleting some characters typed in command mode
                                     left arrow key : to move in the command to delete characters
                                    right arrow key : to move in the command
                                            Any key : to refresh command mode if some status is displayed
up and down arrow keys have been disabled                                   

user can only delete characters from between the command and can insert characters only at end



command format:

	Copy
	  copy <source_file(s)> <destination_directory>
	
	Move
	 move <source_file(s)> <destination_directory>
	
	Rename 
	 rename <old_filename> <new_filename>
    
    creating file
     create_file <file_name> <destination_path> 
     create_dir <dir_name> <destination_path>

    Delete file
     delete_file <file_path>

    Goto
     goto <location> 

    Search
     search <filename or folder name>

    Snapshot 
     snapshot <folder> <dumpfile>

Assumptions:
 1. copy and move command accept multiple file or folder names mixed to be copied. they are similar for directories.
 2. It is assumed that parts of a command have just one space between them with no space in end. If there are more than one spaces the invalid command is prompted to the user 
 3. Allowed file or directory paths:
    absolute paths
    relative paths
 4. ~ means the home folder i.e. the folder where application is started
    . means current folder
    / means home folder i.e. the folder where application is started
    file path with no extra characters means relative path with respect to current folder
    /path : means relative with respect to home
    .. means path to parent folder 

 5. Files or folders with spaces or ' or " in their name should be escaped with \ in command.

 6. Home folder i.e. the folder where application is started forms the boundary of traversal. It does not allow anything to be accessed before it.



EXIT
 To exit the application press q in normal mode
