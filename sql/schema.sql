create table if not exists users(
    user_id text primary key,
    user_name text,
    user_email text,
    user_dob date default '1970-01-01',
    user_bio text,
    passwd text,
    created_at date,
    profile_picture text
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
community_id integer primary key,
name varchar(50) not null,
description text,
icon_image text,
banner_image text,
created_at timestamp,
member_count integer default 0
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

create table if not exists users_communities(
    user_id text,
    community_id integer,
    primary key(user_id, community_id)
);

create table if not exists undelivered_messages(
  message_id serial primary key,
  sender_id integer,
  receiver_id integer,
  sent_at date,
  content text
);

create table if not exists default_image(
  id serial primary key,
  data text not null
);

insert into default_image (data) values ('iVBORw0KGgoAAAANSUhEUgAAADAAAAAwCAYAAABXAvmHAAAACXBIWXMAAAsTAAALEwEAmpwYAAADrElEQVR4nO2Y3WsTWRiHZxUVL0VhwUuxEbzwgyxrFXcm502lLZMpMwnZZUWR9j/YpV4Iih94oV5VjTSiFFFpoLustD0nKctiW2kV3F2xLqjrB7Zq/baxamvBpK+cqYnp2DbJTDoTJT/40eHMnDnPM2cGQgWhlFJKKeWrSXl5+UJCyM8A8DsA3CCEvAUATNXr9XqEYo3H4wkQQgYygY0lhHQKxRgA2DcTuEHif0LIQQD4ViiGEEJ25Apv6GtCiN9ReAD4HgDemxTgu5EAgE2OCRBCzpuFz5AYcLvd82yH93q9a3OFrA1W4KmdlXjvjKz/neKaoO0ChJA9uUD3n5URY8qkRvZWGa9vckKgc6YnbYQ21rATfzshcD/bk87WyKed6LddAABG+eL5QhsLEx/yiBMCWCgBAMCSQD7B5jJ5+5b17wq1Azu2bRjFiKtasCsYcT1INq/Ao/WVk2Aaa8swXFs2LexU50PbK3E8sgKx2XXfVgGMuD4D5HDhuukFpj0fcdkuUK1LMHnc6iuEUTnJ4bHZVWWbQFok6rtpWSCmXLcd/JOAcsL6Dihh5wRiNRWWBZgCjgnoElHfeQsCXY7C6wJUXoYx5bmJV+cldijLhWII0hoRY8pQXvAx+QehmIJ/ymV4bkN2+HPrsWievDEYFhBPLUH8bRViG0Fk1RPlx3yMnwsL9v9wy0sghwrFGvwSBYhK3ZJKQ6JG71y71DuI/e2I/+yZeN9PL50oP+Zj/e147WLvoKjR26LKjoiB6FrHwD01dJ2o0h5JY5hq3a/dPZiRp42N+KShIXMI637pnjRH0tgFKcC+sw28qiq6QNLYcUmj4wYQlFQ2Gn819iIF/9/q1XpTEq+G3w3xa6aYl5RUdmxlsGX+rMJLvrYlksq6PwPI6KFQXxeHfdbUlBbgMjwHjvZ1zTRX0lgvKK2z8z9Tr/rHYlGjN7MAIASiA4lkMpGIx9MC/DiRTCb5uWzzJY1d3yjTRQXGx29ElXXksLjejs6Hl0euXk0LjPT16WO5zhdVRguKL/np1lwX5w3U/XV5qLU1LRBva9PH8rmH6GebCwIfDLbMlVR2N5/F+Uf5776GeErgyv4jw/qHmt89bgm7d88pxNOvyGvhj931U2gwJbDzx9CgmXuIfkqsC6jssJnFQWsfvrLGjbygtb82cw9Jow3WBTR20dziDE/66t+c8NW/NTtfVGmPZQFRZY/NAvgDLY94zQuwx9YFNDpmFsBqRY2OWRYoRZjdfAA67siGQ8wzQwAAAABJRU5ErkJggg==');
