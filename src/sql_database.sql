CREATE TABLE `mailod` (
  `email_id` int(10) NOT NULL auto_increment,
  `body_hash` varchar(64) NOT NULL default '',
  `body_length` varchar(10) NOT NULL default '',
  `inode` int(10) NOT NULL default '0',
  `timestamp` timestamp(14) NOT NULL,
   PRIMARY KEY  (`email_id`)
) TYPE=MyISAM AUTO_INCREMENT=1 ;
