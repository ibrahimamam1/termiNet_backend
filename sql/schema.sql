create table if not exists users(
    user_id serial primary key,
    user_name text,
    user_email text,
    user_sex text,
    user_dob date,
    user_bio text,
    passwd text,
    created_at date
);

create table if not exists undelivered_messages(
  message_id serial primary key,
  sender_id integer,
  receiver_id integer,
  sent_at date,
  content text
);

create table if not exists profile_pics_bucket(
  id serial primary key,
  image text,
  user_id integer references users(user_id)
);
