create sequence if not exists IDSEQ
As Integer
MINVALUE 1
No MAXVALUE
OWNED BY users.user_id;

create sequence if not exists COMMUNITY_SEQ
As Integer
MINVALUE 1
No MAXVALUE
OWNED BY communities.community_id;
