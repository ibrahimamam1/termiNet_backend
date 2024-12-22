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
