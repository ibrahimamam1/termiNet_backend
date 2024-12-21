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

-- Create the trigger
CREATE OR REPLACE TRIGGER new_user_trigger
BEFORE INSERT ON users
FOR EACH ROW
EXECUTE FUNCTION hashPass();
