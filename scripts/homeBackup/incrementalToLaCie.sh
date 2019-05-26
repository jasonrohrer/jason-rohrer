
drivePath="/media/jasonrohrer/LaCie"

date=`date +"%Y_%m%b_%d_%a_%s"`


mostRecent=`ls -1dt $drivePath/* | grep "BACKUP_A" | head -n 1`

latest="$drivePath/BACKUP_A_$date"

# copy with links to most recent files
cp -al $mostRecent $latest

# push date on dir itself forward to now
touch $latest


rsync -axHAX --delete --info=progress2 --modify-window=1 /home/jasonrohrer/backups/BACKUP_A/ $latest