CREATE TABLE `mailod` (
  `email_id` int(10) NOT NULL auto_increment,
  `body_hash` varchar(64) NOT NULL default '',
  `body_length` varchar(10) NOT NULL default '',
  `filesystem` varchar(255) NOT NULL default '',
  `timestamp` timestamp(14) NOT NULL,
  `number_hardlinks` int(5) NOT NULL default '0',
   PRIMARY KEY  (`email_id`)
) TYPE=MyISAM AUTO_INCREMENT=1 ;
