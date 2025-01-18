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


create table if not exists undelivered_messages(
  message_id serial primary key,
  sender_id integer,
  receiver_id integer,
  sent_at date,
  content text
);

