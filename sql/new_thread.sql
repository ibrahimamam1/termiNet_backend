CREATE OR REPLACE FUNCTION updateCommentCount()
RETURNS TRIGGER AS $$
DECLARE
    parentThread integer;
BEGIN
    parentThread := NEW.parent_thread_id;
    IF parentThread IS NOT NULL AND parentThread != 0 THEN
        UPDATE threads 
        SET comment_count = comment_count + 1 
        WHERE thread_id = parentThread;
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER new_comment_trigger
AFTER INSERT ON threads
FOR EACH ROW
WHEN (NEW.parent_thread_id IS NOT NULL AND NEW.parent_thread_id != 0)
EXECUTE FUNCTION updateCommentCount();
