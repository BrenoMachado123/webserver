# webserver
Let's create a simple server using C++
Ok lets do it :)!
***

### Docs

[Simple-webserver-from-scratch](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)
[NGINX webserver configuration file tutorial](https://www.plesk.com/blog/various/nginx-configuration-guide/)
### Guide
 Good Things to Read related with epool:
 .documentation <br>
 .https://copyconstruct.medium.com/the-method-to-epolls-madness-d9d2d6378642 <br>
 .https://www.youtube.com/watch?v=O-yMs3T0APU <br>
 .https://www.youtube.com/watch?v=dEHZb9JsmOU <br>

<details>
	<summary><h3>Directives</h3></summary>


| Directive | Description | Example |
| --------- | ----------- | ------- |
| auth | restrict route to a user | auth user:route |
| autoindex | directory listing on & off | autoindex off \| on  |
| cgi | cgi | cgi |
| cgi-bin | cgi-bin | cgi-bin |
| client_max_body_size | Limit client body size | client_max_body_size #bytes|
| error_page | Setup default error pages | error_page code1 [code2] ... path |
| limit_methods | Define a list of accepted HTTP methods for the route (inside location scope) | limit_methods METHOD |
| listen | Choose the port and host of each ’server' | listen host:port |
| location | Setup routes with one or multiple rules/configuration | location route { ... }|
| root | root | root |
| server_name | Setup the server_names or not | server_name name1 [name2] ...|
| upload | upload | upload |

	

</details>
