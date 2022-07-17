# include "populate_utils.hpp"

void populate_error_map(std::map<int, std::string>& target) {
    target[200] = "OK";
	target[400] = "Bad Request";
	target[403] = "Forbbiden";
	target[404] = "Not Found";
	target[405] = "Method Not Allowed";
	target[413] = "Payload Too Large";
	target[414] = "URI Too Long";
	target[415] = "Unsupported Media Type";
	target[431] = "Request Header Fields Too Large";
	target[500] = "Internal Server Error";
	target[505] = "HTTP Version Not Supported";
}

void populate_mime_map(std::map<std::string, std::string>& target) {
    target["txt"]		= "text/plain";
	target["html"]      = "text/html";
	target["css"]		= "text/css";
	target["js"]		= "text/javascript";
	target["json"]      = "application/json";
	target["jsonld"]	= "application/ld+json";
	target["xml"]		= "application/xml";
	target["pdf"]		= "application/pdf";
	target["doc"]		= "application/msword";
	target["docx"]	    = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	target["ppt"]		= "application/vnd.ms-powerpoint";
	target["pptx"]	    = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	target["odt"]		= "application/vnd.oasis.opendocument.text";
	target["xls"]		= "application/vnd.ms-excel";
	target["xlsx"]	    = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	target["odp"]		= "application/vnd.oasis.opendocument.presentation";
	target["ods"]		= "application/vnd.oasis.opendocument.spreadsheet";

	target["jpeg"]	    = "image/jpeg";
	target["jpg"]		= "image/jpeg";
	target["png"]		= "image/png";
	target["apng"]	    = "image/apng";
	target["avif"]	    = "image/avif";
	target["gif"]		= "image/gif";
	target["svg"]		= "image/svg+xml";
	target["webp"]	    = "image/webp";
	target["webm"]	    = "video/webm";
	target["bmp"]		= "image/bmp";
	target["ico"]		= "image/x-icon";
	target["tif"]		= "image/tiff";
	target["tiff"]	    = "image/tiff";

	target["mp3"]		= "audio/mpeg";
	target["aac"]		= "audio/aac";
	target["wav"]		= "audio/wave";
	target["flac"]	    = "audio/flac";
	target["mpeg"]	    = "audio/mpeg";
	target["mp4"]		= "video/mp4";
	target["avi"]		= "video/x-msvideo";
	target["3gp"]		= "video/3gpp";

	target["bz"]		= "application/x-bzip";
	target["bz2"]		= "application/x-bzip2";
	target["gz"]		= "application/gzip";
	target["zip"]		= "application/zip";
	target["7z"]		= "application/x-7z-compressed";
	target["tar"]		= "application/x-tar";
}

void define_html_content(std::string& target, const std::string& final_path) {
    target = "<html>\n<head><meta content=\"text/html;charset=utf-8\" http-equiv=\"Content-Type\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><meta content=\"utf-8\" http-equiv=\"encoding\"><title>HTTP Autoindex</title><style> \
	div {display: flex; flex-wrap: wrap; justify-content: space-between; max-width: 80%; padding: 0.25rem; border-radius: 0.75rem;} div:hover {background-color: rgba(0, 0, 0, 0.25);} \
	svg {display: inline-block; width: 25px; height: 25px; margin-right: 0.25rem;} a {position: relative; display: inline; vertical-align: top;} .file_name {top: 0; left: 30px; white-space: nowrap;} \
	.flexible {display: flex; flex-direction: row; justify-content: space-around; gap: 1rem;} a:hover .folder .folder-front {transform: translate(0px, 230px) rotateX(60deg);} \
	a:hover .default-file .pencil { display: block; transform: translate(-20px, -35px); animation: 5s draw ease-in infinite; } @keyframes draw { \
	0% {transform: translate(-25px, -30px);} 5% {transform: translate(-20px, -35px);} 10% {transform: translate(-15px, -30px);} 15% {transform: translate(-10px, -35px);} 20% {transform: translate(-5px, -30px);} \
	25% {transform: translate(-0px, -30px);} 30% {transform: translate(-25px, -20px);} 35% {transform: translate(-18px, -25px);} 40% {transform: translate(-11px, -21px);} 45% {transform: translate(-5px, -25px);} \
	50% {transform: translate(-0px, -22px);} 55% {transform: translate(-24px, -15px);} 60% {transform: translate(-21px, -18px);} 65% {transform: translate(-12px, -12px);} 70% {transform: translate(-7px, -17px);} \
	75% {transform: translate(-0px, -12px);} 80% {transform: translate(-5px, -23px);} 90% {transform: translate(-25px, -30px);} 100% {transform: translate(-25px, -30px);} } \
	a:hover .file .top-bar {animation: 2s shrink ease-out;} a:hover .html-file .html-tag {animation: 2s flick ease-out infinite;} \
	@keyframes flick {to {opacity: 0;}} @keyframes shrink {0% {transform: rotateY(25deg) translate(0px,0px);} 70% {transform: rotateY(65deg) translate(40px,0px);} 90% {transform: rotateY(80deg) translate(150px,0px);} 100% {transform: rotateY(85deg) translate(285px,0px);}} \
	</style></head>\n<body>\n<h3>Autoindex for " + final_path + "</h3><hr>";
}

void populate_icons_map(std::map<std::string, std::string>& target) {
    readFileString("utils/folder.svg", target["folder"]);
	readFileString("utils/file.svg", target["file"]);
	readFileString("utils/html_file.svg", target["html"]);
	readFileString("utils/js_file.svg", target["js"]);
	readFileString("utils/css_file.svg", target["css"]);
	readFileString("utils/py_file.svg", target["py"]);
}
