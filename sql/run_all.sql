-- Master script to run all SQL files in order
\echo 'Running database setup scripts...'

\set AUTOCOMMIT off

-- Run scripts in sequence
\i ../sql/schema.sql
\i ../sql/sequence_generators.sql
\i ../sql/triggers.sql
\i ../sql/new_user.sql
\i ../sql/new_thread.sql
\i ../sql/login_check.sql


\set AUTOCOMMIT on

\echo 'All scripts completed!'
