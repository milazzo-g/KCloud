drop database if exists KCloud;

create database KCloud;

create user 'KCloudMan'@'%' IDENTIFIED BY 'kcloud';
grant all privileges on KCloud.* TO 'KCloudMan'@'%' WITH GRANT OPTION;

use KCloud;

create table users(
	email		VARCHAR(255) PRIMARY KEY,
	hash		VARCHAR(32) NOT NULL DEFAULT 'NO HASH',
	space		BIGINT(19) UNSIGNED NOT NULL DEFAULT 0,
	status		ENUM	(
				'Logged',
				'UnLogged'
				) NOT NULL DEFAULT 'UnLogged'
)ENGINE=InnoDB;

create table resources(
	id		BIGINT(19) UNSIGNED ZEROFILL PRIMARY KEY AUTO_INCREMENT,
	parent		BIGINT(19) UNSIGNED DEFAULT 1,
	owner		VARCHAR(255) NOT NULL,
	name		VARCHAR(255) NOT NULL,
	type		ENUM	(
				'Dir',
				'File'
				),
	size		BIGINT(19) UNSIGNED
)ENGINE=InnoDB;

create table publicResources(
	resource	BIGINT(19) UNSIGNED ZEROFILL PRIMARY KEY AUTO_INCREMENT,
	permission	ENUM	(
				'Read',
				'Write'
				)
)ENGINE=InnoDB;

create table sharing(
	user		VARCHAR(255),
	resource	BIGINT(19) UNSIGNED,
	permission	ENUM	(
				'Read',
				'Write'
				),
	PRIMARY KEY(user, resource)
)ENGINE=InnoDB;

insert into users 	(email) values ('admin');
insert into resources	(parent, owner, name, type, size) values (1, 'admin', 'root', 'Dir', 0);


alter table resources
					add constraint fk_01
					foreign key (parent) references resources(id)
					on delete cascade
					on update cascade;

alter table resources
					add constraint fk_02
					foreign key (owner) references users(email)
					on delete cascade
					on update cascade;

alter table sharing
					add constraint fk_03
					foreign key (user) references users(email)
					on delete cascade
					on update cascade;

alter table sharing
					add constraint fk_04
					foreign key (resource) references resources(id)
					on delete cascade
					on update cascade;
alter table publicResources
					add constraint fk_05
					foreign key (resource) references resources(id)
					on delete cascade
					on update cascade;

