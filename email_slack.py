import smtplib
import argparse

parser = argparse.ArgumentParser(description="Used to alert the CMU TechOps Slack channel #totis of a toner below threshold value or upon toner being delivered")
parser.add_argument("type", type=int, help="Type of notification email to send, 1 if at threshold or below, 0 if no toner left. 2 if delivery")
parser.add_argument("toner", type=str, help="The toner name to notify via email")
args = parser.parse_args()


username = "techopstonerinventorysystem"
password = "2FDS24wedfsdF@EYSdfqlwsdf!#$SDF#W@WEDY" #randomly made this password by smashing keyboard...works i guess

server = smtplib.SMTP('smtp.gmail.com:587') #send using gmail
server.starttls() #start server
server.login(username, password) #login

fromaddr = 'TechOpsTonerInventorySystem@gmail.com' #from this email
toaddr1 = "totis.od3bfs@zapiermail.com" #sending to same email as coming from
toaddr2 = "TechOpsTonerInventorySystem@gmail.com"
msg = ""

if(args.type == 1): #this is if toner is at or below 2 units, but still above 0
    msg = "Alert! " + args.toner + " is at or below 2 units. Consider ordering more " + args.toner + " soon."
    #print("type 1 detected\n")
elif(args.type == 0): #this is if toner is at 0 units
    msg = "URGENT! " + args.toner + " is at 0 units! Order more " + args.toner +" ASAP!"
    #print("type 0 detected\n")
elif(args.type == 2): #this is for when toner is delivered (whether or not it is above threshold, just to notify slack)
    msg = "Alert! " + args.toner + " has arrived and is ready to be deployed."
    #print("type 2 detected\n")
else:
    msg = "error"

	
#server.sendmail(fromaddr, toaddr1, msg) #this sends to email account that posts in slack
server.sendmail(fromaddr, toaddr2, msg)

server.quit() #end program
