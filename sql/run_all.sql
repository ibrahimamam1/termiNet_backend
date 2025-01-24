-- Master script to run all SQL files in order
\echo 'Running database setup scripts...'

\set AUTOCOMMIT off

-- Run scripts in sequence
\i ./schema.sql
\i ./sequence_generators.sql
\i ./triggers.sql
\i ./new_user.sql
\i ./new_thread.sql
\i ./login_check.sql


\set AUTOCOMMIT on

\echo 'All scripts completed!'
