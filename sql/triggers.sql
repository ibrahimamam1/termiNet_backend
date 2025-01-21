CREATE OR REPLACE FUNCTION updateMemberCount()
RETURNS TRIGGER AS $$
DECLARE
    c_id integer;
BEGIN
    c_id := NEW.community_id;
    UPDATE communities 
        SET member_count = member_count + 1 
        WHERE community_id = c_id;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER new_community_member_trigger
AFTER INSERT ON users_communities
FOR EACH ROW
EXECUTE FUNCTION updateMemberCount();
