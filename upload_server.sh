sudo cp "workshop.conf" "/etc/apache2/sites-available"

# Simple uploading scripts
sudo cp -a "www/." "/var/www/workshop"

sudo a2ensite workshop
sudo systemctl reload apache2