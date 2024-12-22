CREATE OR REPLACE FUNCTION login_check(email TEXT, p_pass TEXT)
RETURNS BOOLEAN AS $$
DECLARE
    p_hash TEXT;
BEGIN
    SELECT passwd INTO p_hash FROM users WHERE user_email = email;

    -- Check if the user exists
    IF p_hash IS NULL THEN
        RETURN FALSE;
    END IF;

    -- Compare the input password with the stored hash
    IF crypt(p_pass, p_hash) = p_hash THEN
        RETURN TRUE;
    ELSE
        RETURN FALSE;
    END IF;
END;
$$ LANGUAGE plpgsql;

