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
    default_image BYTEA;
    default_image_base64 TEXT;
BEGIN
    -- Load the default profile picture
    default_image := pg_read_binary_file('default_profile.png');
    
    IF default_image IS NULL THEN
        RAISE EXCEPTION 'Could not load default profile picture';
    END IF;
    
    -- Encode the binary data as a base64 string
    default_image_base64 := encode(default_image, 'base64');
    
    NEW.profile_picture = default_image_base64;
    
    RETURN NEW;
EXCEPTION
    WHEN OTHERS THEN
        RAISE EXCEPTION 'Failed to insert default profile picture: %', SQLERRM;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER insert_profile_picture_trigger
BEFORE INSERT ON users
FOR EACH ROW
EXECUTE FUNCTION insertDefaultProfilePicture();
