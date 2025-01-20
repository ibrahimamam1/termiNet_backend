create table if not exists users(
    user_id text primary key,
    user_name text,
    user_email text,
    user_dob date default '1970-01-01',
    user_bio text,
    passwd text,
    created_at date
);

create table if not exists profile_pics_bucket(
  id serial primary key,
  image text,
  user_id text references users(user_id)
);

create table if not exists threads(
  thread_id serial primary key,
  title varchar(200) not null,
  content text,
  created_at timestamp,
  author_id text references users(user_id),
  community_id integer,
  parent_thread_id integer,
  comment_count integer default 0
);

create table if not exists communities(
community_id serial primary key,
name varchar(50) not null,
description text,
icon_image text,
banner_image text,
created_at date
);

create table if not exists iconsBucket(
    icon_id serial primary key,
    filename text not null,
    filedata BYTEA not null,
    uploaded_by integer references users(user_id),
    uploaded_at timestamp default current_timestamp
);

create table if not exists bannersBucket(
    icon_id serial primary key,
    filename text not null,
    filedata BYTEA not null,
    uploaded_by integer references users(user_id),
    uploaded_at timestamp default current_timestamp
);
create table if not exists categories(
    category_id serial primary key,
    category_name text
);
insert into categories(category_name) values ('Art');
insert into categories(category_name) values ('Science');
insert into categories(category_name) values ('Sports');
insert into categories(category_name) values ('Politics');
insert into categories(category_name) values ('News');
insert into categories(category_name) values ('Entertainment');
insert into categories(category_name) values ('Technology');
insert into categories(category_name) values ('Writing');
insert into categories(category_name) values ('Pop culture');
insert into categories(category_name) values ( 'Writing');
insert into categories(category_name) values ( 'Popular Culture');
insert into categories(category_name) values ( 'Anime');
insert into categories(category_name) values ( 'Manga');
insert into categories(category_name) values ( 'Video Games');
insert into categories(category_name) values ( 'Movies');

create table if not exists community_categories(
    community_id integer,
    category_id integer,
    primary key (community_id, category_id)
);


create table if not exists undelivered_messages(
  message_id serial primary key,
  sender_id integer,
  receiver_id integer,
  sent_at date,
  content text
);

