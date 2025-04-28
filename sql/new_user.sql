--Hash Password trigger and function
CREATE OR REPLACE FUNCTION hashPass()
RETURNS TRIGGER AS $$
DECLARE
    password_hash TEXT;
BEGIN
    -- Hash the password from the NEW record
    password_hash := crypt(NEW.passwd, gen_salt('bf', 10));
    NEW.passwd := password_hash;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER new_user_trigger
BEFORE INSERT ON users
FOR EACH ROW
EXECUTE FUNCTION hashPass();

--Insert Default profile Pic Trigger and Function
CREATE OR REPLACE FUNCTION insertDefaultProfilePicture()
RETURNS TRIGGER AS $$
DECLARE
    default_image_base64 TEXT;
BEGIN
    -- Load the default profile picture
    SELECT data
    INTO default_image_base64
    FROM default_image
    WHERE id = 1;
    
    IF default_image_base64 IS NULL THEN
        RAISE EXCEPTION 'Could not load default profile picture';
    END IF;
    
    
    NEW.profile_picture = default_image_base64;
    
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER insert_profile_picture_trigger
BEFORE INSERT ON users
FOR EACH ROW
EXECUTE FUNCTION insertDefaultProfilePicture();
